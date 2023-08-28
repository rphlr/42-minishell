/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:45:19 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/28 12:18:39 by rrouille         ###   ########.fr       */
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

static char	*extract_variable_name(char *ptoken)
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
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, start, len);
	var_name[len] = '\0';
	return (var_name);
}

static int	biggest_var_value(t_env	*env)
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

static char	*format_token(char *token, t_global *global)
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
