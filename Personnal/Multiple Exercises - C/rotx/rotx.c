#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int is_in_low(char i)
{
    if (i >= 'a' && i <= 'z')
        return 1;
    return 0;
}

int is_in_high(char i)
{
    if (i >= 'A' && i <= 'Z')
        return 1;
    return 0;
}

int is_a_num(char i)
{
    if (i >= '0' && i <= '9')
        return 1;
    return 0;
}

void stdintail2(int n)
{
    char string[10000];
    for (unsigned int i = 0; i < 10000; ++i)
        string[i] = 0;

    unsigned int len = read(STDIN_FILENO, string, 10000);

    for (unsigned int i = 0; i < len; ++i)
    {
        int n_ = n;
        if (n_ >= 0)
        {
            if (is_in_low(string[i]))
            {
                while (string[i] + n_ > 255)
                    n_ -= 256;
                string[i] += n_;
                while (!is_in_low(string[i]))
                    string[i] -= 26;
            }
            else if (is_in_high(string[i]))
            {
                while (string[i] + n_ > 255)
                    n_ -= 256;
                string[i] += n_;
                while (!is_in_high(string[i]))
                    string[i] -= 26;
            }
            else if (is_a_num(string[i]))
            {
                while (string[i] + n_ > 255)
                    n_ -= 256;
                string[i] += n_;
                while (!is_a_num(string[i]))
                    string[i] -= 10;
            }
        }
        else
        {
            if (is_in_low(string[i]))
            {
                while (string[i] + n_ < 0)
                    n_ += 256;
                string[i] += n_;
                while (!is_in_low(string[i]))
                    string[i] += 26;
            }
            else if (is_in_high(string[i]))
            {
                while (string[i] + n_ < 0)
                    n_ += 256;
                string[i] += n_;
                while (!is_in_high(string[i]))
                    string[i] += 26;
            }
            else if (is_a_num(string[i]))
            {
                while (string[i] + n_ < 0)
                    n_ += 256;
                string[i] += n_;
                while (!is_a_num(string[i]))
                    string[i] += 10;
            }
        }
    }
    write(1, string, len);
}

int main(int argc, char *argv[])
{
    if (argc > 1)
        stdintail2(atoi(argv[1]));
    return 0;
}
