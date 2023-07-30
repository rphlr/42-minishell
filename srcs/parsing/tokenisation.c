/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 09:02:47 by rrouille          #+#    #+#             */
/*   Updated: 2023/07/30 16:00:16 by rrouille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

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
		if (!ft_strcmp(tokens[i], "|"))
			type[i] = PIPE;
		else if (!ft_strcmp(tokens[i], "<"))
			type[i] = INPUT;
		else if (!ft_strcmp(tokens[i], ">"))
			type[i] = OUTPUT;
		else if (!ft_strcmp(tokens[i], ">>"))
			type[i] = APPEND;
		else if (!ft_strcmp(tokens[i], "<<"))
			type[i] = HEREDOC;
		else if (!ft_strcmp(tokens[i], ";"))
			type[i] = SEMICOLON;
		else if (!ft_strcmp(tokens[i], ":" ))
			type[i] = COLON;
		else if (!ft_strcmp(tokens[i], "&&"))
			type[i] = AND;
		else if (!ft_strcmp(tokens[i], "||"))
			type[i] = OR;
		else if (tokens[i][j] == '$')
			type[i] = VARIABLES;
		else if (tokens[i][0] == '\'')
		{
			j = 1;
			while (tokens[i][j] != '\'')
			{
				if (!tokens[i][j++])
				{
					type[i] = NOT_CLOSED_QUOTE;
					break;
				}
			}
			if (type[i] != NOT_CLOSED_QUOTE)
				type[i] = CLOSED_QUOTE;
		}
		else if (tokens[i][0] == '\"')
		{
			j = 1;
			while (tokens[i][j] != '\"')
			{
				if (!tokens[i][j++])
				{
					type[i] = NOT_CLOSED_DQUOTE;
					break;
				}
			}
			if (type[i] != NOT_CLOSED_DQUOTE)
				type[i] = CLOSED_DQUOTE;
		}
		else
		{
			while (tokens[i][j])
			{
				if (tokens[i][j] == '$')
				{
					type[i] = VARIABLES;
					break;
				}
				else if (tokens[i][j] == '\'')
				{
					type[i] = NOT_CLOSED_QUOTE;
					break;
				}
				else if (tokens[i][j] == '\"')
				{
					type[i] = NOT_CLOSED_DQUOTE;
					break;
				}
				else if (tokens[i][j] == '|')
				{
					type[i] = PIPE;
					break;
				}
				else if (tokens[i][j] == '<')
				{
					type[i] = INPUT;
					break;
				}
				else if (tokens[i][j] == '>')
				{
					type[i] = OUTPUT;
					break;
				}
				else if (tokens[i][j] == ';')
				{
					type[i] = SEMICOLON;
					break;
				}
				else if (tokens[i][j] == ':')
				{
					type[i] = COLON;
					break;
				}
				else if (tokens[i][j] == '&')
				{
					if (tokens[i][j + 1] == '&')
						type[i] = AND;
					else
						type[i] = WORD;
					break;
				}
				else if (tokens[i][j] == '|')
				{
					type[i] = OR;
					break;
				}
				else
					type[i] = WORD;
				j++;
			}
		}
	}
	type[i] = END;
	return (type);
}