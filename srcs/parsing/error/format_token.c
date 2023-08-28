/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 17:14:25 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/28 17:46:07 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quotes(char c, int *in_double_quotes, int *in_simple_quotes)
{
	if (c == '\'' && !(*in_double_quotes))
	{
		*in_simple_quotes = !(*in_simple_quotes);
		return (1);
	}
	if (c == '\"' && !(*in_simple_quotes))
	{
		*in_double_quotes = !(*in_double_quotes);
		return (1);
	}
	return (0);
}

static void	handle_dollar(char **token, t_global *global, int in_simple_quotes,
		char *output, int *i)
{
	char	*num;
	char	*var_name;
	char	*var_value;

	if (in_simple_quotes)
	{
		output[(*i)++] = **token;
		return ;
	}
	(*token)++;
	if (**token == '?')
	{
		global->exit_code = manage_exit(NULL);
		num = ft_itoa(global->exit_code);
		while (*num)
			output[(*i)++] = *num++;
		(*token)++;
	}
	else
	{
		var_name = extract_variable_name(*token);
		if (ft_strlen(var_name) > 0)
		{
			var_value = get_env_value(var_name, global->env);
			while (var_value && *var_value)
				output[(*i)++] = *var_value++;
			*token += ft_strlen(var_name);
		}
		else
			output[(*i)++] = '$';
	}
}

char	*format_token(char *token, t_global *global)
{
	int		in_double_quotes;
	int		in_simple_quotes;
	char	*output;
	int		i;
	int		len_biggest_var_value;

	in_double_quotes = 0;
	in_simple_quotes = 0;
	len_biggest_var_value = biggest_var_value(global->env);
	output = (char *)ft_gc_malloc(len_biggest_var_value + 1);
	if (!output)
		return (NULL);
	i = 0;
	while (*token)
	{
		if (!handle_quotes(*token, &in_double_quotes, &in_simple_quotes))
		{
			if (*token == '$')
				handle_dollar(&token, global, in_simple_quotes, output, &i);
			else
				output[i++] = *token;
		}
		token++;
	}
	output[i] = '\0';
	return (output);
}

// static void	initialize_variables(int *in_double_quotes, int *in_simple_quotes, int *len_biggest_var_value, t_global *global)
// {
// 	*in_double_quotes = 0;
// 	*in_simple_quotes = 0;
// 	*len_biggest_var_value = biggest_var_value(global->env);
// }

// static int	handle_quotes_status(char c, int *in_double_quotes, int *in_simple_quotes)
// {
// 	if (c == '"' && !(*in_simple_quotes))
// 		*in_double_quotes = !(*in_double_quotes);
// 	else if (c == '\'' && !(*in_double_quotes))
// 		*in_simple_quotes = !(*in_simple_quotes);
// 	else
// 		return (0);
// 	return (1);
// }

// static void	process_tokens(char **token, t_global *global, int in_simple_quotes, char *output, int *i)
// {
// 	if (**token == '$')
// 		handle_dollar(token, global, in_simple_quotes, output, i);
// 	else
// 		output[(*i)++] = **token;
// }

// char	*format_token(char *token, t_global *global)
// {
// 	int		in_double_quotes;
// 	int		in_simple_quotes;
// 	char	*output;
// 	int		i;
// 	int		len_biggest_var_value;

// 	initialize_variables(&in_double_quotes, &in_simple_quotes, &len_biggest_var_value, global);
// 	output = (char *)ft_gc_malloc(len_biggest_var_value + 1);
// 	if (!output)
// 		return (NULL);
// 	i = 0;
// 	while (*token)
// 	{
// 		if (!handle_quotes_status(*token, &in_double_quotes, &in_simple_quotes))
// 			process_tokens(&token, global, in_simple_quotes, output, &i);
// 		token++;
// 	}
// 	output[i] = '\0';
// 	return (output);
// }


// static int	handle_quotes(char c, int *in_double_quotes, int *in_simple_quotes)
// {
// 	if (c == '\'' && !(*in_double_quotes))
// 	{
// 		*in_simple_quotes = !(*in_simple_quotes);
// 		return (1);
// 	}
// 	if (c == '\"' && !(*in_simple_quotes))
// 	{
// 		*in_double_quotes = !(*in_double_quotes);
// 		return (1);
// 	}
// 	return (0);
// }

// static void	add_to_output(char *str, char *output, int *i)
// {
// 	while (*str)
// 		output[(*i)++] = *str++;
// }

// static void	handle_dollar_question(char **token, t_global *global, char *output, int *i)
// {
// 	char	*num;

// 	global->exit_code = manage_exit(NULL);
// 	num = ft_itoa(global->exit_code);
// 	add_to_output(num, output, i);
// 	(*token)++;
// }

// static void	handle_dollar_variable(char **token, t_global *global, char *output, int *i)
// {
// 	char	*var_name;
// 	char	*var_value;

// 	var_name = extract_variable_name(*token);
// 	if (ft_strlen(var_name) > 0)
// 	{
// 		var_value = get_env_value(var_name, global->env);
// 		add_to_output(var_value, output, i);
// 		*token += ft_strlen(var_name);
// 	}
// 	else
// 		output[(*i)++] = '$';
// }

// static void	handle_dollar(char **token, t_global *global, int in_simple_quotes,
// 		char *output, int *i)
// {
// 	if (in_simple_quotes)
// 	{
// 		output[(*i)++] = **token;
// 		return ;
// 	}
// 	(*token)++;
// 	if (**token == '?')
// 		handle_dollar_question(token, global, output, i);
// 	else
// 		handle_dollar_variable(token, global, output, i);
// }

// char	*format_token(char *token, t_global *global)
// {
// 	int		in_double_quotes;
// 	int		in_simple_quotes;
// 	char	*output;
// 	int		i;

// 	in_double_quotes = 0;
// 	in_simple_quotes = 0;
// 	output = (char *)ft_gc_malloc(biggest_var_value(global->env) + 1);
// 	if (!output)
// 		return (NULL);
// 	i = 0;
// 	while (*token)
// 	{
// 		if (!handle_quotes(*token, &in_double_quotes, &in_simple_quotes))
// 		{
// 			if (*token == '$')
// 				handle_dollar(&token, global, in_simple_quotes, output, &i);
// 			else
// 				output[i++] = *token;
// 		}
// 		token++;
// 	}
// 	output[i] = '\0';
// 	return (output);
// }
