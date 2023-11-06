#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
    T_IF, /*  if  */
    T_ELSE, /*  else  */
    T_ELIF, /*  elif  */
    T_FI, /*  fi  */
    T_THEN, /*  then  */
    T_SEP, /*  ;  */
    T_WORD, /*  anything else  */
    T_PIPE, /*  |  */
    T_WHILE, /*  while  */
    T_UNTIL, /*  until  */
    T_DO, /*  do  */
    T_DONE, /*  done  */
    T_FOR, /*  for  */
    T_IN, /*  in  */
    T_NEG, /*  !  */
    T_AND, /*  &&  */
    T_OR, /*  ||  */
    T_REDIR_INPUT, /*  <  */
    T_REDIR_OUTPUT, /*  > and >|  */
    T_REDIR_APPEND_OUTPUT, /*  >>  */
    T_REDIR_DUPLI_INPUT, /*  <&  */
    T_REDIR_DUPLI_OUTPUT, /*  >&  */
    T_REDIR_OPEN_FILE, /*  <>  */
    T_D_VAR, /*  var declaration  ie: var=data  */
    T_B_BLK, /*  {  */
    T_E_BLK, /*  }  */
    T_PARA, /*  ()  */
    T_FUNC, /*  function  */
    T_CASE, /*  case */
    T_ESAC, /*  esac */
    T_CASE_COND, /*  condtion for case  */
    T_DOUBLE_SEP, /*  ;;  */
    T_EOF, /*  EOF  */
    T_ERROR, /*  error  */
    T_START_SUB, /*  (*  */
    T_END_SUB, /*  *)  */
    T_S_CSB, /*  $(  */
    T_E_CSB, /*  )  */
    T_CSB, /*  `  */
    T_SUB /* subshell  */
};

struct token
{
    enum token_type type;
    char *var;
    char *data;
};

struct token *new_token(enum token_type type, char *data);

void free_token(struct token *token);

#endif /* !TOKEN_H */
