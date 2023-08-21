/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:42:55 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/18 23:19:10 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_cmd(t_global *global, t_line *line)
{
	char	**token_ptr;
	char	*temp;

	token_ptr = line->token;
	while (*token_ptr)
	{
		temp = NULL;
		if (!ft_strcmp(*token_ptr, "$?"))
			temp = ft_itoa(global->exit_code);
		else if (!ft_strcmp(*token_ptr, "<"))
		{
		}
		else if (!ft_strcmp(*token_ptr, "<<"))
		{
			// heredoc

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
		if (temp)
			*token_ptr = temp;
		else if ((*token_ptr)[0] == '$')
		{
			temp = get_env_value(*token_ptr + 1, global);
			if (temp)
				*token_ptr = ft_strdup(temp);
		}
		token_ptr++;
	}
	return (0);
}
