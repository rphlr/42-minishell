/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 09:02:47 by rrouille          #+#    #+#             */
/*   Updated: 2023/07/26 09:08:11 by rrouille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

t_token	*init_tokens_type(char **tokens)
{
	int		i;
	t_token	*type;

	i = -1;
	type = ft_gc_malloc(sizeof(t_token) * (ft_strlen(*tokens) + 1));
	if (!type)
		return (NULL);
	while (tokens[++i])
	{
		if (!ft_strcmp(tokens[i], "|"))
			type[i] = PIPE;
		else if (!ft_strcmp(tokens[i], "<"))
			type[i] = INPUT;
		else if (!ft_strcmp(tokens[i], ">"))
			type[i] = OUTPUT;
		else if (!ft_strcmp(tokens[i], ">>"))
			type[i] = APPEND;
		else if (!ft_strcmp(tokens[i], ";"))
			type[i] = SEMICOLON;
		else if (tokens[i][0] == '$')
			type[i] = VARIABLES;
		else
			type[i] = WORD;
	}
	type[i] = END;
	return (type);
}