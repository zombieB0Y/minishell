#include "../minishell.h" // Assuming this contains necessary structs and ft_ functions
#include <stdio.h>    // For perror (if not in minishell.h)
#include <string.h>   // For ft_strchr, ft_strlen, ft_memcpy, ft_strncat (or your libft versions)
#include <stdlib.h>   // For gc_malloc, ft_realloc, ft_strdup (or your libft versions)
#include <ctype.h>    // For ft_isalnum, ft_isalpha (or your libft versions)

// Forward declaration for the helper if it's defined later or in another file
static int process_single_expansion(exp_t *exp_data, const char *dollar_ptr, const char *end_of_string, char active_quote_char);
void append_to_buffer(exp_t *exp, const char *str_to_add, size_t add_len); // Ensure const char*
void init_exp(exp_t *new, char *org); // Assuming this initializes exp_t members correctly

void    init_exp(exp_t *new, char *org)
{
    // (void)org; // org might not be used directly by init_exp in the new design
    new->dollar_sign_pos = NULL; // Not used in the quote-aware char-by-char scan
    new->current_pos = org;      // May not be used; new function tracks its own pointer
    new->result_buffer = NULL;
    new->result_len = 0;
    new->result_capacity = 0;
    new->expansions_done = 0;
}

// Your existing append_to_buffer - ensure it handles NULL result_buffer initially
// and reallocates correctly. Using const char* for str_to_add is good practice.
void append_to_buffer(exp_t *exp, const char *str_to_add, size_t add_len) {
    if (add_len == 0)
		return; // Nothing to add

    if (exp->result_buffer == NULL) {
        // Initial allocation: aim for a reasonable default or add_len + 1
        exp->result_capacity = add_len + 1;
        exp->result_buffer = (char *)gc_malloc(exp->result_capacity);
        if (!exp->result_buffer) {
            perror("gc_malloc in append_to_buffer");
            // Handle error appropriately, maybe exit or set an error flag
            return;
        }
        exp->result_buffer[0] = '\0'; // Initialize as empty string
        exp->result_len = 0;
    }

    if (exp->result_len + add_len + 1 > exp->result_capacity) {
        // Need to reallocate
        size_t new_capacity = exp->result_len + add_len + 1;
        // Optional: Add some extra space to reduce frequent reallocs
        if (new_capacity < exp->result_len + add_len + 1)
			new_capacity = exp->result_len + add_len +1;
        char *new_buffer = (char *)ft_realloc(exp->result_buffer, exp->result_capacity, new_capacity);
        if (!new_buffer) {
            perror("ft_realloc in append_to_buffer");
            // Handle error: old buffer is still valid if realloc returns NULL
            // but we can't append.
            return;
        }
        exp->result_buffer = new_buffer;
        exp->result_capacity = new_capacity;
    }

    ft_strncat(exp->result_buffer, str_to_add, add_len); // ft_strncat is safer
    exp->result_len += add_len;
    // ft_strncat should null-terminate if there's space.
    // Ensure result_buffer[result_len] = '\0' if ft_strncat doesn't guarantee for add_len.
    // However, standard strncat appends at most n characters and then a null terminator.
}


/**
 * @brief Processes a single potential variable expansion starting at dollar_ptr.
 *
 * This function encapsulates the logic for identifying a variable name (e.g., $VAR, ${VAR}, $?),
 * getting its value from the environment, and appending it to exp_data->result_buffer.
 * It's called when a '$' is encountered in a context where expansion is allowed.
 *
 * @param exp_data Pointer to the expansion state structure.
 * @param dollar_ptr Pointer to the '$' character in the original string.
 * @param end_of_string Pointer to the null terminator of the original string.
 * @param active_quote_char The active quote character ('"' or 0). Used to determine if inside double quotes.
 * @return The number of characters consumed from the original string (length of $VAR syntax).
 * Returns 1 if '$' is treated literally (e.g., not a valid variable start).
 */
static int process_single_expansion(exp_t *exp_data, const char *dollar_ptr, const char *end_of_string, char active_quote_char) {
    const char *p = dollar_ptr + 1; // Character after '$'
    char *var_name_buffer = NULL;
    char *env_value = NULL;
    int var_name_len = 0;
    const char *var_name_start = p;
    int consumed_len = 1; // Minimum consumed is '$' itself

    if (p >= end_of_string) { // '$' is the last character
        append_to_buffer(exp_data, "$", 1);
        return 1;
    }

    if (*p == '{') { // ${VAR} form
        var_name_start = p + 1;
        const char *closing_brace = ft_strchr(var_name_start, '}');
        if (closing_brace && closing_brace > var_name_start) { // Found '}' and non-empty name
            var_name_len = closing_brace - var_name_start;
            p = closing_brace + 1; // Move p past '}'
            consumed_len = p - dollar_ptr;
        } else { // Malformed ${VAR (no closing brace or empty like ${})
            append_to_buffer(exp_data, "$", 1); // Append '$' literally
            // The '{' and subsequent chars will be handled by the main loop
            return 1; // Only '$' is consumed for this "non-expansion"
        }
    } else if (*p == '?') { // $?
        // env_value = ft_getenv("?"); // Your shell needs to provide this value
        // For demonstration, let's assume ft_getenv handles "?" or you have a specific function
        char *status_str = ft_getenv("?"); // Or get_last_exit_status_str();
        if (status_str) {
            append_to_buffer(exp_data, status_str, ft_strlen(status_str));
            // If status_str was allocated by ft_getenv and is not static, it might need freeing
            // depending on ft_getenv's contract. Assuming ft_getenv returns a string that can be used directly.
        } else {
             append_to_buffer(exp_data, "0", 1); // Default if not found or error
        }
        exp_data->expansions_done++;
        consumed_len = 2; // Consumed "$?"
        return consumed_len;
    } else if (ft_isalpha(*p) || *p == '_') { // $VAR form (starts with letter or underscore)
        var_name_start = p;
        while (p < end_of_string && (ft_isalnum(*p) || *p == '_')) {
            p++;
        }
        var_name_len = p - var_name_start;
        consumed_len = p - dollar_ptr;
    } else { // '$' followed by non-variable character (e.g., "$$", "$1", "$<space>")
        append_to_buffer(exp_data, "$", 1);
        return 1; // Only '$' is consumed, the next char will be handled by main loop.
    }

    if (var_name_len > 0) {
        var_name_buffer = (char *)gc_malloc(var_name_len + 1);
        if (!var_name_buffer) {
            perror("gc_malloc for var_name_buffer");
            append_to_buffer(exp_data, "$", 1); // Fallback: treat $ literally
            // And append the original variable text if possible, or just skip.
            // This is an error condition. For simplicity, just append '$'.
            return 1;
        }
        ft_memcpy(var_name_buffer, var_name_start, var_name_len);
        var_name_buffer[var_name_len] = '\0';

        env_value = ft_getenv(var_name_buffer);
        if (env_value != NULL) {
            append_to_buffer(exp_data, env_value, ft_strlen(env_value));
        }
        // If env_value is NULL, bash substitutes with an empty string (which append_to_buffer handles by doing nothing for NULL).
        // Or, if ft_getenv can return NULL for unset, and append_to_buffer(..., NULL, 0) is safe.
        // My append_to_buffer above handles add_len == 0. If env_value is NULL, ft_strlen(NULL) is a crash.
        // So ensure ft_getenv returns "" for unset, or handle NULL from ft_getenv here.
        // Corrected: if (env_value != NULL) { append_to_buffer(exp_data, env_value, ft_strlen(env_value)); }

        gc_free(var_name_buffer); // Free the temporary var_name_buffer
        exp_data->expansions_done++;
    } else if (!(*dollar_ptr == '$' && dollar_ptr[1] == '?')) { // If not $? and no var_name_len, it was malformed
        // This case should be covered by the malformed ${VAR} path above.
        // If it reaches here, it means var_name_len was 0 for $VAR form, which is unlikely
        // if it passed the ft_isalpha check.
        // If it was a form like "$$" that didn't match any rule, $ was already appended.
    }
    return consumed_len;
}

/**
 * @brief Expands variables in a string, respecting quoting rules.
 *
 * Single quotes ('') prevent any expansion within them.
 * Double quotes ("") allow expansion but also handle escaped characters like \$, \", \\.
 * Unquoted strings allow expansion.
 *
 * @param original_value The string to expand.
 * @return A new string with variables expanded (allocated with gc_malloc),
 * or the original_value if no expansions occurred or if original_value is NULL/empty.
 * The caller is responsible for freeing the returned new string if it's different from original_value,
 * assuming a garbage collector isn't handling token->value automatically upon reassignment.
 * Given gc_malloc, if new string is returned, old token->value (if also gc_malloc'd)
 * should be implicitly handled by GC if no other refs.
 */
char *expand_string_variables(char *original_value) {
    if (!original_value || !*original_value) {
        return original_value;
    }

    exp_t exp_data;
    init_exp(&exp_data, original_value); // Pass original_value for consistency, though not strictly used by this init_exp

    const char *current_char_ptr = original_value;
    const char *segment_start_ptr = original_value; // Start of the current literal segment
    char active_quote_char = 0; // 0 for none, '\'' for single, '"' for double
    const char *end_of_string = original_value + ft_strlen(original_value);

    while (current_char_ptr < end_of_string) {
        if (active_quote_char == '\'') { // Inside single quotes
            if (*current_char_ptr == '\'') {
                active_quote_char = 0; // End of single quote
                // Segment includes the closing quote. Let it be appended with the segment.
                current_char_ptr++; // Move past the quote
            } else {
                current_char_ptr++; // Move to next char
            }
        } else if (active_quote_char == '"') { // Inside double quotes
            if (*current_char_ptr == '"') {
                active_quote_char = 0; // End of double quote
                current_char_ptr++;
            } else if (*current_char_ptr == '\\' && current_char_ptr + 1 < end_of_string &&
                       (current_char_ptr[1] == '$' || current_char_ptr[1] == '"' ||
                        current_char_ptr[1] == '\\' || current_char_ptr[1] == '`')) {
                // Append segment before the backslash
                if (current_char_ptr > segment_start_ptr) {
                    append_to_buffer(&exp_data, segment_start_ptr, current_char_ptr - segment_start_ptr);
                }
                // Append the escaped character itself (e.g., '$' not the '\$')
                append_to_buffer(&exp_data, current_char_ptr + 1, 1);
                current_char_ptr += 2; // Move past '\' and the escaped character
                segment_start_ptr = current_char_ptr; // New segment starts after
            } else if (*current_char_ptr == '$') {
                // Append segment before the '$'
                if (current_char_ptr > segment_start_ptr) {
                    append_to_buffer(&exp_data, segment_start_ptr, current_char_ptr - segment_start_ptr);
                }
                int consumed = process_single_expansion(&exp_data, current_char_ptr, end_of_string, active_quote_char);
                current_char_ptr += consumed;
                segment_start_ptr = current_char_ptr; // New segment starts after expansion
            } else { // Regular character inside double quotes
                current_char_ptr++;
            }
        } else { // Not in any quote (active_quote_char == 0)
            if (*current_char_ptr == '\'' || *current_char_ptr == '"') {
                // Append segment before this quote
                if (current_char_ptr > segment_start_ptr) {
                    append_to_buffer(&exp_data, segment_start_ptr, current_char_ptr - segment_start_ptr);
                }
                active_quote_char = *current_char_ptr;
                append_to_buffer(&exp_data, current_char_ptr, 1); // Append the quote char itself
                current_char_ptr++;
                segment_start_ptr = current_char_ptr; // New segment starts after this quote
            } else if (*current_char_ptr == '$') {
                // Append segment before the '$'
                if (current_char_ptr > segment_start_ptr) {
                    append_to_buffer(&exp_data, segment_start_ptr, current_char_ptr - segment_start_ptr);
                }
                int consumed = process_single_expansion(&exp_data, current_char_ptr, end_of_string, active_quote_char);
                current_char_ptr += consumed;
                segment_start_ptr = current_char_ptr; // New segment starts after expansion
            } else { // Regular character, not in quote, not a special char
                current_char_ptr++;
            }
        }
    }

    // Append any remaining segment at the end of the string
    if (current_char_ptr > segment_start_ptr) {
        append_to_buffer(&exp_data, segment_start_ptr, current_char_ptr - segment_start_ptr);
    }
    
    // If unclosed quote, bash typically leaves it as is. Our logic does this by appending.

    if (exp_data.expansions_done == 0) {
        // No actual variable replacements took place.
        // Free the buffer if it was allocated for copying, and return original.
        if (exp_data.result_buffer != NULL) {
            gc_free(exp_data.result_buffer);
        }
        return original_value;
    } else {
        if (exp_data.result_buffer == NULL)
            return ft_strdup("");
        return exp_data.result_buffer;
    }
}

token_list_t	*expand(token_list_t *tokens) {
    if (!tokens) return NULL;
    lol *current_node = tokens->head;
    char *new_value;
    token_t *token;

    while (current_node) {
        token = current_node->token;
        if (token && token->type == TOKEN_WORD && token->value != NULL) {
            // Only attempt expansion if a '$' is present in the token's value.
            // The quote-awareness is now handled by expand_string_variables.
            if (ft_strchr(token->value, '$')) {
                new_value = expand_string_variables(token->value);

                if (new_value != token->value) {
                    token->value = new_value;
                }
            }
        }
        current_node = current_node->next;
    }
    return tokens;
}
