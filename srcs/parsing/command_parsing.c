/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:42:55 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/22 13:27:44 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_cmd(t_global *global, t_line *line)
{
	char	**token_ptr;
	char	*temp;

	(void) global;
	token_ptr = line->token;
	while (*token_ptr)
	{
		temp = NULL;
		if (!ft_strcmp(*token_ptr, "<"))
		{
		}
		else if (!ft_strcmp(*token_ptr, "<<"))
		{
		}
		else if (!ft_strcmp(*token_ptr, ">"))
		{
		}
		else if (!ft_strcmp(*token_ptr, ">>"))
		{
		}
		else if (!ft_strcmp(*token_ptr, "|"))
		{
		}
		token_ptr++;
	}
	return (0);
}
