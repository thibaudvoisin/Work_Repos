#include "lexer.h"

#include <ctype.h>
#include <err.h>
#include <fnmatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/alloc.h"

// newline = 0 -> pas de word avant ; 1 -> word avant
int newline = 0;
int is_quote = 0; // check for variable (ie: echo 3=3 is not a variable)
int in_case = 0;
int back_quote = 0;
int in_csb = 0;

static void remove_space(struct lexer **lexer)
{
    while ((strlen((*lexer)->input) > (*lexer)->pos)
           && isspace((*lexer)->input[(*lexer)->pos])
           && (*lexer)->input[(*lexer)->pos] != '\n')
        (*lexer)->pos++;
}

static int check_valid_char_first(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        return 1;
    return 0;
}

static int check_valid_char(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'
        || (c >= 0 && c <= '9'))
        return 1;
    return 0;
}

static int check_var(char *word)
{
    int code = 1;
    int i = 0;
    for (; word[i] != '\0' && code; i++)
    {
        if (!i)
            code = check_valid_char_first(word[i]);
        else if (word[i] == '=')
            break;
        else
            code = check_valid_char(word[i]);
    }
    if (word[i] == '\0')
        return 0;
    return code;
}

static int check_func(char *word)
{
    int code = 1;
    int i = 0;
    for (; word[i] != '\0' && code; i++)
    {
        if (!i)
            code = check_valid_char_first(word[i]);
        else
        {
            if (word[i] == ')' || word[i] == '(')
                continue;
            code = check_valid_char(word[i]);
        }
    }
    return code;
}

static enum token_type check_word_type(char *current_word)
{
    enum token_type tok_type = T_WORD;

    if (((strlen(current_word) == 2 && current_word[0] == '!')
         && current_word[1] == '=')
        || (is_quote && !in_case))
    {
        is_quote = 0;
        return tok_type;
    }

    if ((strlen(current_word) == 1 && current_word[0] == '=')
        || (is_quote && !in_case))
    {
        is_quote = 0;
        return tok_type;
    }
    if (!current_word)
        tok_type = T_ERROR;

    else if (!strcmp(current_word, "if"))
        tok_type = T_IF;

    else if (!strcmp(current_word, "elif"))
        tok_type = T_ELIF;

    else if (!strcmp(current_word, "else"))
        tok_type = T_ELSE;

    else if (!strcmp(current_word, "fi"))
        tok_type = T_FI;

    else if (!strcmp(current_word, "then"))
        tok_type = T_THEN;

    else if ((!strcmp(current_word, ";")) || (!strcmp(current_word, "\n")))
        tok_type = T_SEP;

    else if (!strcmp(current_word, "|"))
        tok_type = T_PIPE;

    else if (!strcmp(current_word, "while"))
        tok_type = T_WHILE;

    else if (!strcmp(current_word, "for"))
        tok_type = T_FOR;

    else if (!strcmp(current_word, "until"))
        tok_type = T_UNTIL;

    else if (!strcmp(current_word, "do"))
        tok_type = T_DO;

    else if (!strcmp(current_word, "in"))
        tok_type = T_IN;

    else if (!strcmp(current_word, "done"))
        tok_type = T_DONE;

    else if (!strcmp(current_word, "!"))
        tok_type = T_NEG;

    else if (!strcmp(current_word, "&&"))
        tok_type = T_AND;

    else if (!strcmp(current_word, "||"))
        tok_type = T_OR;
    // if there is an = in current word:
    else if (check_var(current_word))
        tok_type = T_D_VAR;

    else if ((!fnmatch("[0-9]<", current_word, 0))
             || (!strcmp("<", current_word)))
        tok_type = T_REDIR_INPUT;

    else if ((!fnmatch("[0-9]>", current_word, 0))
             || (!strcmp(">", current_word)))
        tok_type = T_REDIR_OUTPUT;

    else if ((!fnmatch("[0-9]>|", current_word, 0))
             || (!strcmp(">|", current_word)))
        tok_type = T_REDIR_OUTPUT;

    else if ((!fnmatch("[0-9]>>", current_word, 0))
             || (!strcmp(">>", current_word)))
        tok_type = T_REDIR_APPEND_OUTPUT;

    else if ((!fnmatch("[0-9]<&", current_word, 0))
             || (!strcmp("<&", current_word)))
        tok_type = T_REDIR_DUPLI_INPUT;

    else if ((!fnmatch("[0-9]>&", current_word, 0))
             || (!strcmp(">&", current_word)))
        tok_type = T_REDIR_DUPLI_OUTPUT;

    else if ((!fnmatch("[0-9]<>", current_word, 0))
             || (!strcmp("<>", current_word)))
        tok_type = T_REDIR_OPEN_FILE;

    else if (!strcmp(current_word, "{"))
        tok_type = T_B_BLK;

    else if (!strcmp(current_word, "}"))
        tok_type = T_E_BLK;

    else if (!strcmp(current_word, "()"))
        tok_type = T_PARA;

    else if (!strcmp(current_word, "case"))
    {
        in_case = 1;
        tok_type = T_CASE;
    }

    else if (!strcmp(current_word, "esac"))
    {
        in_case = 0;
        tok_type = T_ESAC;
    }

    else if (in_case && strlen(current_word) > 1
             && current_word[strlen(current_word) - 1] == ')')
        tok_type = T_CASE_COND;
    else if (check_func(current_word) && strlen(current_word) > 2
             && current_word[strlen(current_word) - 2] == '('
             && current_word[strlen(current_word) - 1] == ')')
        tok_type = T_FUNC;

    else if (!fnmatch("(*)", current_word, 0))
        tok_type = T_SUB;

    else if (current_word[0] == '(')
        tok_type = T_START_SUB;

    else if (!strcmp(current_word, ")") && in_csb)
    {
        in_csb = 0;
        tok_type = T_E_CSB;
    }

    else if (strlen(current_word) >= 1
             && current_word[strlen(current_word) - 1] == ')')
        tok_type = T_END_SUB;

    else if (!strcmp(current_word, "`"))
        tok_type = T_CSB;

    else if (!strcmp(current_word, "$("))
        tok_type = T_S_CSB;

    else if (!strcmp(current_word, "\0"))
        tok_type = T_EOF;

    return tok_type;
}

static void check_redir(struct lexer **lexer)
{
    if ((*lexer)->input[(*lexer)->pos] == '>')
    {
        if ((*lexer)->input[(*lexer)->pos + 1] == '<')
            errx(2, "Syntax error on redirection: '>.<'");
        if ((*lexer)->input[(*lexer)->pos + 1] == '>'
            || (*lexer)->input[(*lexer)->pos + 1] == '|'
            || (*lexer)->input[(*lexer)->pos + 1] == '&')
            (*lexer)->pos++;
    }
    else if ((*lexer)->input[(*lexer)->pos] == '<')
    {
        if ((*lexer)->input[(*lexer)->pos + 1] == '|')
            errx(2, "Syntax error on redir, une pipe unexpected");
        if ((*lexer)->input[(*lexer)->pos + 1] == '>'
            || (*lexer)->input[(*lexer)->pos + 1] == '&')
            (*lexer)->pos++;
    }
}

static char *get_current_word(struct lexer *lexer)
{
    // get the current word in the lexer input
    // remove space

    remove_space(&lexer);
    size_t start_pos = lexer->pos;
    int quote = 0;

    if ((strlen(lexer->input) <= lexer->pos))
        return strdup("\0");

    if (lexer->input[lexer->pos] == '\'' || lexer->input[lexer->pos] == '\"')
    {
        lexer->pos++;
        quote = 1;
        is_quote = 1;
        while (lexer->pos < strlen(lexer->input)
               && lexer->input[lexer->pos] != '\''
               && lexer->input[lexer->pos] != '\"')
            lexer->pos++;
    }
    else
    {
        while (lexer->input[lexer->pos] != '\0'
               && lexer->input[lexer->pos] != ' '
               && lexer->input[lexer->pos] != ';'
               && lexer->input[lexer->pos] != '\n'
               && lexer->input[lexer->pos] != '>'
               && lexer->input[lexer->pos] != '<'
               && lexer->input[lexer->pos] != '`')
        {
            lexer->pos++;
        }
    }

    char *word = calloc(lexer->pos - start_pos + 4, sizeof(char));

    if (quote)
    {
        newline = 1;
        word = strncpy(word, (lexer->input + start_pos),
                       lexer->pos - start_pos + 1);
        if (in_case)
        {
            lexer->pos++;
            remove_space(&lexer);
            if (lexer->input[lexer->pos] == ')')
            {
                word = strcat(word, ")");
                lexer->pos++; // need to be modified if necessary
            }
            else
                lexer->pos--;
        }
    }
    else if (lexer->pos == start_pos
             && (lexer->input[lexer->pos] == ';'
                 || lexer->input[lexer->pos] == '>'
                 || lexer->input[lexer->pos] == '<'
                 || lexer->input[lexer->pos] == '`' || newline))
    {
        // check case when we have a ; alone (a semicolon between spaces)
        newline = 0;
        check_redir(&lexer);
        word = strncpy(word, (lexer->input + start_pos),
                       lexer->pos - start_pos + 1);
    }
    else if (lexer->pos == start_pos && !newline)
    {
        // check case when we have ;\n
        lexer->pos++;
        free(word);
        return get_current_word(lexer);
    }
    else
    {
        if (lexer->input[lexer->pos] == ';')
        {
            /* check case when a word is close to a semicolon (ex: hello;)
             ** we need to decrease the pos for the next token (i.e. we do
             ** lexer->pos++ in get_next_token), but we steel want the last
             ** character (in the ex above, we want to keep 'o')
             */
            lexer->pos--;
            word = strncpy(word, (lexer->input + start_pos),
                           lexer->pos - start_pos + 1);
        }
        else if (lexer->input[lexer->pos] == '\n')
        {
            lexer->pos--;
            word = strncpy(word, (lexer->input + start_pos),
                           lexer->pos - start_pos + 1);

            enum token_type word_type = check_word_type(word);

            if (word_type == T_WORD || word_type == T_FI || word_type == T_D_VAR
                || word_type == T_DONE
                || word_type == T_ESAC) // check mot d'avant //
            {
                newline = 1;
                strcpy(word + strlen(word), "\0");
                return word;
            }
            else
            {
                lexer->pos++;
                newline = 0;
                strcpy(word + strlen(word), "\0");
                return word;
            }
        } // lex redirection
        else if (lexer->input[lexer->pos] == '>'
                 || lexer->input[lexer->pos] == '<')
        {
            // check if we have more than 1 char before > (ex: 45>), or a
            // non-digit char before >
            if (lexer->pos - start_pos >= 2
                || !isdigit(lexer->input[lexer->pos - 1]))
            {
                word = strncpy(word, (lexer->input + start_pos),
                               lexer->pos - start_pos);
                lexer->pos--;
            }
            else
            {
                check_redir(&lexer);
                word = strncpy(word, (lexer->input + start_pos),
                               lexer->pos - start_pos + 1);
            }
        }
        else
        {
            // I have removed + 1 because it printed an undesirable extra
            // character
            word = strncpy(word, (lexer->input + start_pos),
                           lexer->pos - start_pos);
        }
    }
    if (!strcmp(word, "(){"))
    {
        word[strlen(word) - 1] = '\0';
        lexer->pos -= 2;
    }
    else if (strlen(word) > 1 && word[strlen(word) - 1] == ')' && in_csb)
    {
        // in_csb = 0;
        word[strlen(word) - 1] = '\0';
        if (lexer->input[lexer->pos] == ' ')
            lexer->pos--;
        lexer->pos--;
    }
    strcpy(word + strlen(word), "\0");

    // printf("WORD:  %s ----  LEN: %ld\n", word, strlen(word));
    return word;
}

static struct lexer *get_current_token(struct lexer *lexer, char *current_word)
{
    // get the current full word
    // check is token type
    // create a new_token with token type and data word
    // lexer->token->type = type
    // lexer->token->data = word
    // return lexer

    free(lexer->token->var);
    free(lexer->token->data);

    // if we got a T_D_VAR we need to lex it to return the var name and the data

    if (strlen(current_word) > 2 && current_word[0] == '$'
        && current_word[1] == '(')
    {
        size_t size = strlen(current_word);
        for (size_t i = 2; i < size; i++)
            current_word[i] = '\0';
        in_csb = 1;
        lexer->pos -= size - 1;
    }

    enum token_type tok_type = check_word_type(current_word);
    if (tok_type == T_D_VAR)
    {
        lexer->token->type = tok_type;

        size_t var_pos = 0;
        char *var_name = calloc(strlen(current_word) + 2, sizeof(char));
        while (var_pos < strlen(current_word) && current_word[var_pos] != '=')
        {
            var_name[var_pos] = current_word[var_pos];
            var_pos++;
        }

        lexer->token->var = var_name;
        var_pos++;

        char *data = calloc(strlen(current_word) + 1, sizeof(char));
        for (; var_pos < strlen(current_word); var_pos++)
            data[var_pos - strlen(var_name) - 1] = current_word[var_pos];
        free(current_word);
        lexer->token->data = data;

        return lexer;
    }

    lexer->token->type = tok_type;
    lexer->token->data = current_word;
    lexer->token->var = NULL;
    return lexer;
}

// init the lexer with the current input
// ie: argv[1] or argv[2] in the "-c" case
struct lexer *new_lexer(char *input)
{
    struct lexer *lexer = calloc(1, sizeof(struct lexer));
    lexer->input = input;
    lexer->pos = 0;
    lexer->token = calloc(1, sizeof(struct token));
    remove_space(&lexer);
    char *current_word = get_current_word(lexer);
    // get_current_token the first word and change the token

    lexer = get_current_token(lexer, current_word);

    return lexer;
}

void free_lexer(struct lexer *lexer)
{
    free(lexer);
}

static struct lexer *duplicate_lexer(struct lexer *lexer)
{
    struct lexer *new = calloc(1, sizeof(struct lexer));

    char *input_cpy = zalloc(strlen(lexer->input) + 1);
    strcpy(input_cpy, lexer->input);
    new->input = input_cpy;

    new->pos = (lexer->pos);

    struct token *token_cpy = zalloc(sizeof(struct token));
    enum token_type type_cpy = lexer->token->type;
    if (type_cpy != T_EOF && lexer->token->data)
    {
        char *data = zalloc(strlen(lexer->token->data) + 1);
        strcpy(data, lexer->token->data);
        token_cpy->data = data;
    }

    if (lexer->token->var)
    {
        char *var = zalloc(strlen(lexer->token->var) + 1);
        strcpy(var, lexer->token->var);
        token_cpy->var = var;
    }

    token_cpy->type = type_cpy;
    new->token = token_cpy;
    return new;
}

// get the current token without deleting it
// if you use peek_lexer in a row it will give always give you the same token
struct token *peek_lexer(struct lexer *lexer)
{
    struct lexer *tmp = duplicate_lexer(lexer);

    tmp->pos--;

    struct token *tmp_token = get_next_token(&tmp);
    free_token(tmp_token);

    if (lexer->token->type == T_WORD && check_func(lexer->token->data)
        && tmp->token->type == T_PARA)
        lexer->token->type = T_FUNC;

    struct token *token = zalloc(sizeof(struct token));
    token->type = lexer->token->type;
    token->data = NULL;
    token->var = NULL;

    if (token->type != T_EOF && token->type != T_ERROR)

    {
        token->data = strdup(lexer->token->data);
        if (lexer->token->var)
            token->var = strdup(lexer->token->var);
    }
    free_token(tmp->token);
    free(tmp->input);
    free(tmp);
    return token;
}

// "pop" the current roken and return it
// also load the next token to be parsed
// you need to stop parsing when you encounter a T_EOF or T_ERROR token type
// ie: the command is finished or the lexer encounter an error in the execution
struct token *get_next_token(struct lexer **lexer)
{
    struct lexer *tmp = duplicate_lexer(*lexer);
    // go to next token
    // change pos
    // get_current_token the new token
    (*lexer)->pos++;
    char *current_word = get_current_word(*lexer);
    *lexer = get_current_token(*lexer, current_word);

    if (tmp->token->type == T_WORD && check_func(tmp->token->data)
        && (*lexer)->token->type == T_PARA)
        tmp->token->type = T_FUNC;

    // the next token is a separator beacause fi need a separator
    // need to be checked in parse instead of here TODO
    if (tmp->token->type == T_FI
        || tmp->token->type == T_DONE) // || tmp->token->type == T_DONE
    {
        // if current token is a fi then force the next token to be a T_SEP
        // if next token is not alredy one
        // (*lexer)->token->type != T_SEP OLD VERSION
        if ((*lexer)->token->type == T_FI || (*lexer)->token->type == T_THEN
            || (*lexer)->token->type == T_ELIF
            || (*lexer)->token->type == T_ELSE || (*lexer)->token->type == T_DO
            || (*lexer)->token->type == T_DONE)
        {
            (*lexer)->pos = tmp->pos;
            (*lexer)->token->type = T_SEP;
        }
    }

    struct token *tok_ret = zalloc(sizeof(struct token));
    tok_ret->type = tmp->token->type;
    if (tok_ret->type != T_ERROR && tok_ret->type != T_EOF)
    {
        if (tmp->token->var != NULL)
            tok_ret->var = strdup(tmp->token->var);
        tok_ret->data = strdup(tmp->token->data);
    }
    free_token(tmp->token);
    free(tmp->input);
    free(tmp);

    is_quote = 0;

    return tok_ret;
}
