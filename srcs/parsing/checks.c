/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:39:22 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/28 00:41:52 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	check_options_doublon(char *token)
// {
// 	char	*start;
// 	char	*compare;

// 	start = token;
// 	while (*start)
// 	{
// 		compare = start + 1;
// 		while (*compare)
// 		{
// 			if (*start == *compare)
// 				return (1);
// 			compare++;
// 		}
// 		start++;
// 	}
// 	return (0);
// }

int	check_options_syntax(char *token)
{
	while (*++token)
	{
		if (!ft_isalpha(*token) && !ft_isdigit(*token))
			return (1);
	}
	return (0);
}

t_state	check_token_errors(t_token *type, char **tokens,
		t_state *error_table)
{
	while (*type != END)
	{
		if (*type >= PIPE && *type <= SEMICOLON && (*(type + 1) == *type
				|| *(type + 1) == END))
			return (error_table[*type]);
		if (*type == OPTIONS)
		{
			if (check_options_syntax(*tokens))
				return (OPTIONS_ERROR);
		}
		type++;
		tokens++;
	}
	return (VALID);
}

bool	check_token(char *line)
{
	if (!line)
	{
		ft_printf("exit\n");
		return (false);
	}
	return (true);
}

t_state	check_errors(t_token *type, char **tokens, t_global *global)
{
	t_state	state;

	state = ft_error(type, tokens, global);
	if (state == QUOTE_ERROR)
		ft_printf("minishell: quote error\n");
	else if (state == DQUOTE_ERROR)
		ft_printf("minishell: dquote error\n");
	else if (state == PIPE_ERROR)
		ft_printf("minishell: syntax error near unexpected token `|'\n");
	else if (state == INPUT_ERROR)
	{
		if (!ft_strcmp(tokens[0], "<"))
			ft_printf(\
			"minishell: syntax error near unexpected token `newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `<'\n");
	}
	else if (state == OUTPUT_ERROR)
	{
		if (!ft_strcmp(tokens[0], ">"))
			ft_printf("minishell: syntax error near unexpected token `newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `>'\n");
	}
	else if (state == APPEND_ERROR)
	{
		if (!ft_strcmp(tokens[0], ">>"))
			ft_printf(\
			"minishell: syntax error near unexpected token `newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `>>'\n");
	}
	else if (state == HEREDOC_ERROR)
	{
		if (!ft_strcmp(tokens[0], "<<"))
			ft_printf(\
			"minishell: syntax error near unexpected token `newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `<<'\n");
	}
	else if (state == AND_ERROR)
	{
		if (!ft_strcmp(tokens[0], "&"))
			ft_printf("minishell: syntax error near unexpected token `&'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `&&'\n");
	}
	else if (state == OR_ERROR)
		ft_printf("minishell: syntax error near unexpected token `||'\n");
	else if (state == SEMICOLON_ERROR)
		ft_printf("minishell: syntax error near unexpected token `;'\n");
	else if (state == OPTIONS_ERROR)
		ft_printf("option error `%s'\n", tokens[0]);
	if (state != VALID)
		return (state);
	return (VALID);
}
