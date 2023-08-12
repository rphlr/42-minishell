/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:42:55 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/12 17:35:54 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_cmd(t_global *global, t_cmd *cmd)
{
	char	**token_ptr;
	char	*temp;

	token_ptr = cmd->token;
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
			temp = getenv(*token_ptr + 1);
			if (temp)
				*token_ptr = ft_strdup(temp);
		}
		token_ptr++;
	}
	return (0);
}

// int	parse_cmd(t_global *global, t_cmd *cmd)
// {
// 	char	**token_ptr;

// 	token_ptr = cmd->token;
// 	while (*token_ptr)
// 	{
// 		if (!ft_strcmp(*token_ptr, "$HOME"))
// 			*token_ptr = global->env->home;
// 		else if (!ft_strcmp(*token_ptr, "$PWD"))
// 			*token_ptr = global->env->pwd;
// 		else if (!ft_strcmp(*token_ptr, "$OLDPWD"))
// 			*token_ptr = global->env->oldpwd;
// 		else if (!ft_strcmp(*token_ptr, "$USER"))
// 			*token_ptr = global->env->user;
// 		else if (!ft_strcmp(*token_ptr, "$SHELL"))
// 			*token_ptr = global->env->shell;
// 		else if (!ft_strcmp(*token_ptr, "$?"))
// 			*token_ptr = ft_itoa(global->exit_code);
// 		else if (!ft_strcmp(*token_ptr, "<"))
// 		{
// 		}
// 		else if (!ft_strcmp(*token_ptr, "<<"))
// 		{
// 		}
// 		else if (!ft_strcmp(*token_ptr, ">"))
// 		{
// 		}
// 		else if (!ft_strcmp(*token_ptr, ">>"))
// 		{
// 		}
// 		else if (!ft_strcmp(*token_ptr, "|"))
// 		{
// 		}
// 		token_ptr++;
// 	}
// 	return (0);
// }
