/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 09:02:47 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/07 17:21:13 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	detect_type(char **tokens, int i, t_token *type)
{
	int	j;

	j = 0;
	while (tokens[i][j])
	{
		if (tokens[i][j] == '$')
			type[i] = DOLLAR;
		else if (tokens[i][j] == '\'')
			type[i] = NOT_CLOSED_QUOTE;
		else if (tokens[i][j] == '\"')
			type[i] = NOT_CLOSED_DQUOTE;
		else if (tokens[i][j] == '|')
			type[i] = PIPE;
		else if (tokens[i][j] == '<')
			type[i] = INPUT;
		else if (tokens[i][j] == '>')
			type[i] = OUTPUT;
		else if (tokens[i][j] == ';')
			type[i] = SEMICOLON;
		else if (tokens[i][j] == ':')
			type[i] = COLON;
		else if (tokens[i][j] == '&')
		{
			if (tokens[i][j + 1] == '&')
				type[i] = AND;
			else
				type[i] = WORD;
		}
		else if (tokens[i][j] == '|')
			type[i] = OR;
		else
			type[i] = WORD;
		j++;
	}
}

t_token	*init_tokens_type(char **tokens)
{
	int		i;
	int		j;
	t_token	*type;

	i = -1;
	type = ft_gc_malloc(sizeof(t_token) * (ft_tablen(tokens) + 1));
	if (!type)
		return (NULL);
	while (tokens[++i])
	{
		j = 0;
		if (tokens[i][j] == '|')
		{
			while (tokens[i][j] == '|')
				j++;
			if (j == 1)
				type[i] = PIPE;
			else
				type[i] = OR;
		}
		else if (tokens[i][j] == '<')
		{
			while (tokens[i][j] == '<')
				j++;
			if (j == 1 || j == 3)
				type[i] = INPUT;
			else
				type[i] = HEREDOC;
		}
		else if (tokens[i][j] == '>')
		{
			while (tokens[i][j] == '>')
				j++;
			if (j == 1 || j == 3)
				type[i] = OUTPUT;
			else
				type[i] = APPEND;
		}
		else if (!ft_strcmp(tokens[i], ";"))
			type[i] = SEMICOLON;
		else if (!ft_strcmp(tokens[i], ":" ))
			type[i] = COLON;
		else if (tokens[i][j] == '&')
			type[i] = AND;
		else if (tokens[i][j] == '$')
			type[i] = DOLLAR;
		else if (tokens[i][j] == '~')
			type[i] = TILDE;
		else if (tokens[i][j] == '*')
			type[i] = STAR;
		else if (tokens[i][j] == '\'')
		{
			j = 1;
			while (tokens[i][j] != '\'')
			{
				if (!tokens[i][j++])
				{
					type[i] = NOT_CLOSED_QUOTE;
					break ;
				}
			}
			if (type[i] != NOT_CLOSED_QUOTE)
				type[i] = CLOSED_QUOTE;
		}
		else if (tokens[i][j] == '\"')
		{
			j = 1;
			while (tokens[i][j] != '\"')
			{
				if (!tokens[i][j++])
				{
					type[i] = NOT_CLOSED_DQUOTE;
					break ;
				}
			}
			if (type[i] != NOT_CLOSED_DQUOTE)
				type[i] = CLOSED_DQUOTE;
		}
		else if (tokens[i][j] == '-')
		{
			if (tokens[i][j + 1] == '-')
				type[i] = WORD;
			else
				type[i] = OPTIONS;
		}
		else
			detect_type(tokens, i, type);
	}
	type[i] = END;
	return (type);
}
