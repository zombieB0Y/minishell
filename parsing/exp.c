#include "../minishell.h"

void expand(lol *token_list_head)
{
    lol *current_node = token_list_head;
    char *new_value;
    token_t *token;

    while (current_node->token->type != TOKEN_EOF) 
    {
        token = current_node->token;
        if (token->type == TOKEN_WORD) 
        {
            if (token->quote != '\'')
            {
                new_value = expand_string_variables(token->value);
                if (new_value != token->value)
                    token->value = new_value;
            }
        }
        current_node = current_node->next;
    }
}

exp_t	*init_exp(char *org)
{
	exp_t	*new = (exp_t *)gc_malloc(sizeof(exp_t));
	if (new)
		return NULL;
	new->dollar_sign_pos = 0;
	new->current_pos = org;
	new->result_buffer = NULL;
	new->result_len = 0;
	new->result_capacity = 0;
	new->expansions_done = 0;
	return (new);
}

char *expand_string_variables(char *original_value)
{
    if (!original_value)
		return NULL;
	exp_t	*exp = init_exp(original_value);
	if (!exp)
		return (NULL);
	size_t prefix_len;

    while ((exp->dollar_sign_pos = ft_strchr(exp->current_pos, '$')) != NULL)
	{
        exp->expansions_done++;

        prefix_len = exp->dollar_sign_pos - exp->current_pos;
        if (prefix_len > 0)
        {
            append_to_buffer(exp, exp->current_pos, prefix_len);
        }

        char var_name_buffer[256];
        int i = 0;
        char *var_char_ptr = dollar_sign_pos + 1;

        bool curly_braces = false;
        if (*var_char_ptr == '{') {
            curly_braces = true;
            var_char_ptr++;
        }

        while (isalnum(*var_char_ptr) || *var_char_ptr == '_') {
            if (i < sizeof(var_name_buffer) - 1) {
                var_name_buffer[i++] = *var_char_ptr;
            }
            var_char_ptr++;
        }
        var_name_buffer[i] = '\0';

        if (curly_braces) {
            if (*var_char_ptr == '}') {
                var_char_ptr++;
            } else {
                append_to_buffer(&result_buffer, &result_len, &result_capacity, dollar_sign_pos, (var_char_ptr - dollar_sign_pos));
                current_pos = var_char_ptr;
                continue;
            }
        }

        if (strlen(var_name_buffer) > 0) {
            char *env_value = ft_getenv(var_name_buffer);
            if (env_value != NULL) {
                append_to_buffer(&result_buffer, &result_len, &result_capacity, env_value, strlen(env_value));
            }
        } else if (*(dollar_sign_pos + 1) != '\0' && !isspace(*(dollar_sign_pos + 1))) {
            if (*(dollar_sign_pos + 1) == '\0' || isspace(*(dollar_sign_pos + 1)) || (!isalpha(*(dollar_sign_pos + 1)) && *(dollar_sign_pos + 1) != '_')) {
                append_to_buffer(&result_buffer, &result_len, &result_capacity, "$", 1);
            } else {
                char temp[3] = {'$', *(dollar_sign_pos + 1), '\0'};
                if (*(dollar_sign_pos + 1) == '\0') {
                    append_to_buffer(&result_buffer, &result_len, &result_capacity, temp, 1);
                } else {
                    append_to_buffer(&result_buffer, &result_len, &result_capacity, temp, 2);
                }
                if (*(dollar_sign_pos + 1) != '\0') {
                    var_char_ptr = dollar_sign_pos + 2;
                } else {
                    var_char_ptr = dollar_sign_pos + 1;
                }
            }
        } else {
            append_to_buffer(&result_buffer, &result_len, &result_capacity, "$", 1);
        }

        current_pos = var_char_ptr;
    }

    if (strlen(current_pos) > 0) {
        append_to_buffer(&result_buffer, &result_len, &result_capacity, current_pos, strlen(current_pos));
    }

    if (expansions_done == 0) {
        if (result_buffer) free(result_buffer);
        return (char *)original_value;
    } else {
        if (!result_buffer) {
            result_buffer = strdup("");
        }
        return result_buffer;
    }
}

// void append_to_buffer(char **buffer, size_t *len, size_t *capacity, const char *str_to_add, size_t add_len) {
//     if (*buffer == NULL) {
//         if (add_len + 1 > 256) {
//             *capacity = add_len + 1;
//         } else {
//             *capacity = 256;
//         }
//         *buffer = (char *)malloc(*capacity);
void append_to_buffer(exp_t *exp, char *str_to_add, size_t add_len)
{
    if (exp->result_buffer == NULL)
    {
        exp->result_buffer = (char *)gc_malloc(exp->result_capacity);
        if (!exp->result_buffer) 
        {
            perror("malloc");
            return ;
        }
        exp->result_len = 0;
        exp->result_buffer[0] = '\0';
    }
    if (exp->result_len + add_len + 1 > exp->result_capacity)
    {
        exp->result_capacity = (exp->result_len + add_len + 1) * 2;
        char *new_buffer = (char *)ft_realloc(exp->result_buffer, exp->result_capacity,);
        if (!new_buffer) {
            perror("realloc");
            return ;
        }
        exp->result_buffer = new_buffer;
    }
    ft_strncat(exp->result_buffer, str_to_add, add_len);
    exp->result_len += add_len;
}
