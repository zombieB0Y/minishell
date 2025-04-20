#include "minishell.h"

int	check(char *p)
{
    // --- test sdfasfas\s
	if (!is_whitespace(*p) && *p != '\'' && *p != '"' && *p != '|' && *p != '>' && *p != '<' && *p != '(')
    {
        if (*p == '\\' && *(p + 1) && (*(p + 1) == '\'' || *(p + 1) == '"' ))
            ft_memmove(p, p + 1, ft_strlen(p));
		return (0);
    }
	return (1);
}
