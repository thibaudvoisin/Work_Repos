#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void prntnum(unsigned long n, int base, char sign, char *outbuf)
{
    int i = 12;
    int j = 0;

    do
    {
        outbuf[i] = "0123456789ABCDEF"[n % base];
        i--;
        n = n / base;
    } while (n > 0);

    if (sign != ' ')
    {
        outbuf[0] = sign;
        ++j;
    }

    while (++i < 13)
    {
        outbuf[j++] = outbuf[i];
    }

    outbuf[j] = 0;
}

int strlenn(char *s)
{
    int i = 0;
    for (; s[i]; ++i)
        continue;
    return i;
}

int dump_ints(int *arr, const char *path)
{
    int f = open(path, O_WRONLY | O_APPEND | O_CREAT);

    if (f == (-1))
        return 0;

    for (int i = 0; i < 7; ++i)
    {
        unsigned char buf[sizeof(int)];

        for (unsigned int gh = 0; gh < sizeof(int); ++gh)
            buf[gh] = 0;
        for (unsigned int j = 0; j < sizeof(int); ++j)
        {
            unsigned char u = arr[i];
            buf[j] = u;
            arr[i] = arr[i] >> 8;
        }

        char str[1000];
        for (unsigned int pl = 0; pl < 1000; ++pl)
            str[pl] = 0;

        unsigned char *pin = buf;
        const char *hex = "0123456789abcdef";
        char *pout = str;
        for (; pin < buf + sizeof(int); pout += 4, pin++)
        {
            pout[1] = hex[((*pin) >> 4) & 0xF];
            pout[0] = hex[*pin & 0xF];
            pout[3] = '\\';
            pout[2] = 'x';
        }
        str[sizeof(int) * 4] = 0;

        int strlenn = 0;

        for (; str[strlenn]; ++strlenn)
            continue;

        char str_2[1000];
        for (unsigned int pl = 0; pl < 1000; ++pl)
            str_2[pl] = 0;

        for (int h = strlenn - 1; h >= 0; h--)
        {
            str_2[h] = str[strlenn - h - 1];
        }
        int strlenn2 = 0;

        for (; str_2[strlenn2]; ++strlenn2)
            continue;

        write(f, str_2, strlenn2);
    }
    close(f);
    return 1;
}
int hex2int(char *hex)
{
    int val = 0;
    while (*hex)
    {
        // get current character then increment
        unsigned char byte = *hex++;
        // transform hex character to the 4bit equivalent number, using the
        // ascii table indexes
        if (byte >= '0' && byte <= '9')
            byte = byte - '0';
        else if (byte >= 'a' && byte <= 'f')
            byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <= 'F')
            byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new
        // digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

int trans(int a, int b, int c, int d)
{
    int res = 0;
    res += (16777216 * a);
    res += (65536 * b);
    res += (256 * c);
    res += (d);

    return res;
}

int read_ints(int *arr, const char *path)
{
    int f = open(path, O_RDONLY);

    if (f == (-1))
        return 0;

    char str[10000];
    for (int i = 0; i < 10000; ++i)
    {
        str[i] = 0;
    }
    unsigned int strlen_ = read(f, str, 10000);
    if (!strlen_)
        return 0;

    for (int i = 0; i < 7; ++i)
    {
        unsigned char fd_1 = str[(i * 16) + 2];
        unsigned char fd_2 = str[(i * 16) + 3];
        unsigned char sd_1 = str[(i * 16) + 6];
        unsigned char sd_2 = str[(i * 16) + 7];
        unsigned char td_1 = str[(i * 16) + 10];
        unsigned char td_2 = str[(i * 16) + 11];
        unsigned char gd_1 = str[(i * 16) + 14];
        unsigned char gd_2 = str[(i * 16) + 15];
        char fd[3];
        fd[0] = fd_1;
        fd[1] = fd_2;
        fd[2] = 0;

        char *hex = fd;
        int val = 0;
        while (*hex)
        {
            // get current character then increment
            unsigned char byte = *hex++;
            // transform hex character to the 4bit equivalent number, using the
            // ascii table indexes
            if (byte >= '0' && byte <= '9')
                byte = byte - '0';
            else if (byte >= 'a' && byte <= 'f')
                byte = byte - 'a' + 10;
            else if (byte >= 'A' && byte <= 'F')
                byte = byte - 'A' + 10;
            // shift 4 to make space for new digit, and add the 4 bits of the
            // new digit
            val = (val << 4) | (byte & 0xF);
        }
        int tmp1 = val;

        char sd[3];
        sd[0] = sd_1;
        sd[1] = sd_2;
        sd[2] = 0;

        char *hex2 = sd;
        int val2 = 0;
        while (*hex2)
        {
            // get current character then increment
            unsigned char byte2 = *hex2++;
            // transform hex character to the 4bit equivalent number, using the
            // ascii table indexes
            if (byte2 >= '0' && byte2 <= '9')
                byte2 = byte2 - '0';
            else if (byte2 >= 'a' && byte2 <= 'f')
                byte2 = byte2 - 'a' + 10;
            else if (byte2 >= 'A' && byte2 <= 'F')
                byte2 = byte2 - 'A' + 10;
            // shift 4 to make space for new digit, and add the 4 bits of the
            // new digit
            val2 = (val2 << 4) | (byte2 & 0xF);
        }
        int tmp2 = val2;

        char td[3];
        td[0] = td_1;
        td[1] = td_2;
        td[2] = 0;

        char *hex3 = td;
        int val3 = 0;
        while (*hex3)
        {
            // get current character then increment
            unsigned char byte3 = *hex3++;
            // transform hex character to the 4bit equivalent number, using the
            // ascii table indexes
            if (byte3 >= '0' && byte3 <= '9')
                byte3 = byte3 - '0';
            else if (byte3 >= 'a' && byte3 <= 'f')
                byte3 = byte3 - 'a' + 10;
            else if (byte3 >= 'A' && byte3 <= 'F')
                byte3 = byte3 - 'A' + 10;
            // shift 4 to make space for new digit, and add the 4 bits of the
            // new digit
            val3 = (val3 << 4) | (byte3 & 0xF);
        }
        int tmp3 = val3;

        char gd[3];
        gd[0] = gd_1;
        gd[1] = gd_2;
        gd[2] = 0;

        char *hex4 = gd;
        int val4 = 0;
        while (*hex4)
        {
            // get current character then increment
            unsigned char byte4 = *hex4++;
            // transform hex character to the 4bit equivalent number, using the
            // ascii table indexes
            if (byte4 >= '0' && byte4 <= '9')
                byte4 = byte4 - '0';
            else if (byte4 >= 'a' && byte4 <= 'f')
                byte4 = byte4 - 'a' + 10;
            else if (byte4 >= 'A' && byte4 <= 'F')
                byte4 = byte4 - 'A' + 10;
            // shift 4 to make space for new digit, and add the 4 bits of the
            // new digit
            val4 = (val4 << 4) | (byte4 & 0xF);
        }
        int tmp4 = val4;

        arr[i] = trans(tmp1, tmp2, tmp3, tmp4);
    }
    close(f);
    return 1;
}
/*
int main()
{
    int *arr = calloc(7, sizeof(int));

    arr[0] = -1;
    arr[1] = 44;
    arr[2] = 121;
    arr[3] = 41564;
    arr[4] = 7;
    arr[5] = 53484;
    arr[6] = 123123;

    dump_ints(arr, "file");
    int *arr2 = calloc(7, sizeof(int));
    read_ints(arr2, "file");
    for (int i = 0; i < 7; ++i)
        printf("->%d\n", arr2[i]);
    free(arr);
    free(arr2);
    return 1;
}*/
