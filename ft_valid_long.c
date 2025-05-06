#include "minishell.h"

int is_valid_llong(const char *str)
{
    int i = 0;
    int sign = 1;
    long long num = 0;
    long long overflow;

    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
        i++;
    if (sign == 1)
        overflow = LLONG_MAX / 10;
    else
        overflow = (LLONG_MIN / 10) * -1;
    while (str[i])
    {
        if (num > overflow) {
            return 0;
        }
        num *= 10;
        if (sign == 1)
        {
            if (num > LLONG_MAX - (str[i] - '0'))
                return (0);
        } 
        else 
        {
            if (-num < LLONG_MIN + (str[i] - '0'))
                return (0);
        }
        num += str[i] - '0';
        i++;
    }
    return (1);
}
