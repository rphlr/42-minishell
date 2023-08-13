/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:39:22 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/13 17:11:44 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_token(char *line)
{
	if (!line)
	{
		ft_printf("Error while loading argument, exiting...\n");
		return (false);
	}
	return (true);
}

t_state	check_errors(t_token *type, char **tokens)
{
	t_state	state;

	state = validity_maker(type, tokens);
	if (state == QUOTE_ERROR)
		ft_printf("minishell: quote error\n");
	else if (state == DQUOTE_ERROR)
		ft_printf("minishell: dquote error\n");
	else if (state == PIPE_ERROR)
		ft_printf("minishell: syntax error near unexpected token `|'\n");
	else if (state == INPUT_ERROR)
	{
		if (!ft_strcmp(tokens[0], "<"))
			ft_printf("\
			minishell: syntax error near unexpected token `newline'\n");
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
			ft_printf("minishell: syntax error near unexpected token `newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `>>'\n");
	}
	else if (state == HEREDOC_ERROR)
	{
		if (!ft_strcmp(tokens[0], "<<"))
			ft_printf("minishell: syntax error near unexpected token `newline'\n");
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
