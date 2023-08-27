/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:45:19 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/27 07:59:48 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_state	*get_error_table(void)
{
	t_state	*error_table;

	error_table = ft_gc_malloc(8 * sizeof(t_state));
	if (!error_table)
		return (NULL);
	error_table[PIPE] = PIPE_ERROR;
	error_table[INPUT] = INPUT_ERROR;
	error_table[OUTPUT] = OUTPUT_ERROR;
	error_table[APPEND] = APPEND_ERROR;
	error_table[HEREDOC] = HEREDOC_ERROR;
	error_table[AND] = AND_ERROR;
	error_table[OR] = OR_ERROR;
	error_table[SEMICOLON] = SEMICOLON_ERROR;
	return (error_table);
}

char	*extract_variable_name(char *ptoken)
{
	char	*start;
	int		len;
	char	*var_name;

	start = ptoken;
	while (*ptoken && ((*ptoken >= 'a' && *ptoken <= 'z') || (*ptoken >= 'A'
				&& *ptoken <= 'Z') || (*ptoken >= '0' && *ptoken <= '9')
			|| *ptoken == '_'))
		ptoken++;
	len = ptoken - start;
	var_name = (char *)ft_gc_malloc(len + 1);
	ft_strncpy(var_name, start, len);
	var_name[len] = '\0';
	return (var_name);
}

int	biggest_var_value(t_env	*env)
{
	int		len;
	int		len_biggest;
	t_env	*current;

	len_biggest = 0;
	current = env;
	while (current)
	{
		len = ft_strlen(current->value);
		if (len > len_biggest)
			len_biggest = len;
		current = current->next;
	}
	return (len_biggest);
}

char	*format_token(char *token, t_global *global)
{
	int		in_double_quotes;
	int		in_simple_quotes;
	char	*output;
	int		i;
	int		len_biggest_var_value;
	char	*num;
	char	*var_name;
	char	*var_value;

	in_double_quotes = 0;
	in_simple_quotes = 0;
	len_biggest_var_value = biggest_var_value(global->env);
	output = (char *)ft_gc_malloc(len_biggest_var_value + 1);
	if (!output)
		return (NULL);
	i = 0;
	while (*token)
	{
		if (*token == '\'' && !in_double_quotes)
		{
			in_simple_quotes = !in_simple_quotes;
			token++;
			continue ;
		}
		if (*token == '\"' && !in_simple_quotes)
		{
			in_double_quotes = !in_double_quotes;
			token++;
			continue ;
		}
		if (*token == '$')
		{
			if (in_simple_quotes)
			{
				output[i++] = *token++;
				continue ;
			}
			token++;
			if (*token == '?')
			{
				global->exit_code = manage_exit(NULL);
				num = ft_itoa(global->exit_code);
				while (*num)
					output[i++] = *num++;
				token++;
			}
			else
			{
				var_name = extract_variable_name(token);
				if (ft_strlen(var_name) > 0)
				{
					var_value = get_env_value(var_name, global->env);
					if (var_value)
					{
						while (*var_value)
							output[i++] = *var_value++;
					}
					token += ft_strlen(var_name);
				}
				else
					output[i++] = '$';
			}
		}
		else
			output[i++] = *token++;
	}
	output[i] = '\0';
	return (output);
}

// char **merge_adjacent_quotes(char **tokens)
// {
//     char **temp_tokens = tokens; // Use a temporary pointer for iteration

//     while (*temp_tokens)
//     {
// 		printf("current: %s\n", *temp_tokens);
//         char *current = *temp_tokens;
//         char *next = *(temp_tokens + 1);
        
//         // If current token is a single or double quote and the next token is the same
//         if ((current && current[0] == '\'' && current[1] == '\0' && next && next[0] == '\'') ||
//             (current && current[0] == '\"' && current[1] == '\0' && next && next[0] == '\"'))
//         {
// 			printf("merge\n");
// 			printf("current: %s\n", *temp_tokens);
// 			printf("next: %s\n", *(temp_tokens + 1));
//             // Merge the tokens
//             *temp_tokens = ""; // Empty the current token
//             *(temp_tokens + 1) = ""; // Empty the next token
//             temp_tokens += 2; // Move two steps ahead with the temporary pointer
//         }
//         else
//         {
//             temp_tokens++; // Move one step ahead with the temporary pointer
//         }
//     }

//     return tokens; // Return the original pointer
// }




// char **split_tokens_with_multiple_quotes(char **tokens)
// {
//     int total_tokens = 0;
//     while (tokens[total_tokens])
//         total_tokens++;

//     for (int i = 0; i < total_tokens; i++)
//     {
//         char *token = tokens[i];

//         // Count the number of quotes in the token.
//         int quote_count = 0;
//         for (int j = 0; token[j]; j++)
//             if (token[j] == '"' || token[j] == '\'')
//                 quote_count++;

//         // If there are more than 2 quotes, the token contains multiple quoted strings.
//         if (quote_count > 2)
//         {
//             char *start = token;
//             char *end = start;
//             int new_tokens_count = 0;

//             while (*end)
//             {
//                 if (*end == '"' || *end == '\'')
//                 {
//                     char quote = *end;
//                     end++;

//                     while (*end && *end != quote)
//                         end++;

//                     if (*end == quote)
//                         end++;

//                     new_tokens_count++;
//                 }
//                 else
//                 {
//                     end++;
//                 }
//             }

//             // Create space for the new tokens.
//             char **new_tokens = (char **)ft_gc_malloc((total_tokens + new_tokens_count + 1) * sizeof(char *));
//             int idx = 0;
//             for (int j = 0; j < i; j++)
//                 new_tokens[idx++] = tokens[j];

//             end = start;
//             while (*end)
//             {
//                 if (*end == '"' || *end == '\'')
//                 {
//                     char quote = *end;
//                     start = end++;
//                     while (*end && *end != quote)
//                         end++;
//                     if (*end == quote)
//                         end++;

//                     new_tokens[idx++] = ft_strndup(start, end - start);
//                 }
//                 else
//                 {
//                     end++;
//                 }
//             }

//             for (int j = i + 1; j < total_tokens; j++)
//                 new_tokens[idx++] = tokens[j];

//             new_tokens[idx] = NULL;
//             tokens = new_tokens;

//             // Adjust the total token count.
//             total_tokens += new_tokens_count - 1;
//         }
//     }

//     return tokens;
// }


t_state	ft_error(t_token *type, char **tokens, t_global *global)
{
	int		nbr_quote;
	int		nbr_dquote;
	t_state	*error_table;
	t_state	token_check_result;

	nbr_quote = 0;
	nbr_dquote = 0;
	error_table = get_error_table();
	token_check_result = check_token_errors(type, tokens, error_table);
	if (token_check_result != VALID)
		return (token_check_result);
	while (*type != END)
	{
		*tokens = format_token(*tokens, global);
		nbr_quote += (*type == NOT_CLOSED_QUOTE) + 2 * (*type == CLOSED_QUOTE);
		nbr_dquote += (*type == NOT_CLOSED_DQUOTE) + 2
			* (*type == CLOSED_DQUOTE);
		type++;
		tokens++;
	}
	if (nbr_quote % 2 != 0)
		return (QUOTE_ERROR);
	if (nbr_dquote % 2 != 0)
		return (DQUOTE_ERROR);
	return (VALID);
}
