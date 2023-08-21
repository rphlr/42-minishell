/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:45:19 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/21 17:34:40 by rrouille         ###   ########.fr       */
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

// char	*format_token(char *token, t_global *global)
// {
// 	int		in_double_quotes;
// 	int		in_simple_quotes;
// 	char	*end = NULL;
// 	char	temp;
// 	// char	*tempr = token;
// 	char	*var_value;
// 	char	*str = token;

// 	in_double_quotes = 0;
// 	in_simple_quotes = 0;
// 	int i = 0;
// 	while (*token)
// 	{
// 		printf("token: %s\n", token);
// 		if (*token == '\\' && !in_double_quotes)
// 		{
// 			token++;
// 			if (in_simple_quotes)
// 			{
// 				if (*token == 'n')
// 					str[i] = '\n';
// 				else if (*token == 't')
// 					str[i] = '\t';
// 				else if (*token == 'v')
// 					str[i] = '\v';
// 				else if (*token == 'b')
// 					str[i] = '\b';
// 				else if (*token == 'r')
// 					str[i] = '\r';
// 				else if (*token == 'f')
// 					str[i] = '\f';
// 				else if (*token == 'a')
// 					str[i] = '\a';
// 				else if (*token == '0')
// 					str[i] = '\0';
// 				else if (*token == '\\')
// 					str[i] = '\\';
// 				else
// 					continue;
// 			}
// 			// token++;
// 			i++;
// 			continue ;
// 		}
// 		if (*token == '\'' && !in_simple_quotes)
// 		{
// 			in_double_quotes = !in_double_quotes;
// 			token++;
// 			continue ;
// 		}
// 		else if (*token == '\"' && !in_double_quotes)
// 		{
// 			in_simple_quotes = !in_simple_quotes;
// 			token++;
// 			continue ;
// 		}
// 		if (in_double_quotes)
// 		{
// 			token++;
// 			continue ;
// 		}
// 		if (*token == '$' && !in_double_quotes)
// 		{
// 			token++;
// 			if (*token == '?')
// 			{
// 				char *num = ft_itoa(global->exit_code);
// 				while (*num)
// 					str[i++] = *num++;
// 			}
// 			else if ((*token >= 'a' && *token <= 'z') || (*token >= 'A'
// 					&& *token <= 'Z') || (*token >= '0' && *token <= '9')
// 				|| *token == '_')
// 			{
// 				end = token;
// 				while (*end && ((*end >= 'a' && *end <= 'z') || (*end >= 'A'
// 							&& *end <= 'Z') || (*end >= '0' && *end <= '9')
// 						|| *end == '_'))
// 					end++;
// 				temp = *end;
// 				*end = '\0';
// 				var_value = get_env_value(token, global);
// 				if (var_value)
// 				{
// 					while (*var_value)
// 						str[i++] = *var_value++;
// 				};
// 				*end = temp;
// 				token = end;
// 			}
// 			// *str++ = *token++;
// 			i++;
// 		}
// 		else
// 			str[i++] = *token++;
// 	}
// 	str[i] = '\0';
// 	printf("str: %s\n", str);
// 	return (str);
// }

char get_escaped_char(char c) {
	switch (c) {
		case 'n': return '\n';
		case 't': return '\t';
		case 'v': return '\v';
		case 'b': return '\b';
		case 'r': return '\r';
		case 'f': return '\f';
		case 'a': return '\a';
		case '0': return '\0';
		case '\\': return '\\';
		default: return 0; // Indicating no special character
	}
}

char *extract_variable_name(char *ptoken) {
	char *start = ptoken;
	while (*ptoken && (
		(*ptoken >= 'a' && *ptoken <= 'z') ||
		(*ptoken >= 'A' && *ptoken <= 'Z') ||
		(*ptoken >= '0' && *ptoken <= '9') ||
		*ptoken == '_')) {
		ptoken++;
	}
	int len = ptoken - start;
	char *var_name = (char*)ft_gc_malloc(len + 1);
	ft_strncpy(var_name, start, len);
	var_name[len] = '\0';
	return var_name;
}

char *format_token(char *token, t_global *global) {
	int in_double_quotes = 0;
	int in_simple_quotes = 0;
	char *output = (char*)ft_gc_malloc(strlen(token) * 2 + 1); // Assuming worst case: every char is a var name
	int i = 0;

	while (*token) {
		if (*token == '\\' && !in_double_quotes) {
			token++;
			char escaped_char = get_escaped_char(*token);
			if (in_simple_quotes && escaped_char) {
				output[i++] = escaped_char;
				token++;
				continue;
			} else if (escaped_char) {
				// If there's an escape sequence outside of double quotes, ignore the backslash and use the character as is.
				output[i++] = *token++;
				continue;
			}
		}
		if (*token == '\'' && !in_double_quotes) {
			in_simple_quotes = !in_simple_quotes;
			output[i++] = *token++;
			continue;
		}
		if (*token == '\"' && !in_simple_quotes) {
			in_double_quotes = !in_double_quotes;
			output[i++] = *token++;
			continue;
		}
		if (in_double_quotes) {
			output[i++] = *token++;
			continue;
		}
		if (*token == '$') {
			if (*(token + 1) == '?') {
				// if (in_simple_quotes) {
					char *num = ft_itoa(global->exit_code);
					while (*num) {
						output[i++] = *num++;
					}
				// } else {
				// 	output[i++] = *token++;
				// 	output[i++] = *token++;
				// }
				token++;
				token++;
			} else {
				char *var_name = extract_variable_name(token);
				char *var_value = get_env_value(var_name, global);
				if (var_value) {
					while (*var_value) {
						output[i++] = *var_value++;
					}
				}
			}
		} else {
			output[i++] = *token++;
		}
	}
	output[i] = '\0';
	return output;
}

t_state	ft_error(t_token *type, char **tokens, t_global *global)
{
	int		nbr_quote;
	int		nbr_dquote;
	t_state	*error_table;
	t_state	token_check_result;

	// (void)global;

	nbr_quote = 0;
	nbr_dquote = 0;
	error_table = get_error_table();
	token_check_result = check_token_errors(type, tokens, error_table);
	if (token_check_result != VALID)
		return (token_check_result);
	while (*type != END)
	{
		// nbr_quote = 0;
		// nbr_dquote = 0;
		// in_double_quotes = 0;
		// in_simple_quotes = 0;
		// char *str = *tokens;
		// char *tempr = *tokens;
		// printf("str: %s\n", str);
		*tokens = format_token(*tokens, global);
		nbr_quote += (*type == NOT_CLOSED_QUOTE) + 2 * (*type == CLOSED_QUOTE);
		nbr_dquote += (*type == NOT_CLOSED_DQUOTE) + 2
			* (*type == CLOSED_DQUOTE);
		type++;
		// *tokens = tempr;
		tokens++;
	}
	if (nbr_quote % 2 != 0)
		return (QUOTE_ERROR);
	if (nbr_dquote % 2 != 0)
		return (DQUOTE_ERROR);
	return (VALID);
}
