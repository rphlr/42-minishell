/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:40:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/27 15:34:31 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	define_home_folder(t_global *global)
{
	if (global->env == NULL)
		return ;
	global->line->cmds->home_folder = get_env_value("HOME", global);
}

void	ft_cd(t_global *global)
{
	char	*path;

	if (global->line->token[1] == NULL)
	{
		path = get_env_value("HOME", global);
		if (path == NULL)
			ft_printf("minishell: cd: HOME not set\n");
		else if (chdir(path) == -1)
			ft_printf("minishell: cd: %s: %s\n", path, strerror(errno));
	}
	else if (global->line->token[2] != NULL)
		ft_printf("minishell: cd: too many arguments\n");
	else if (chdir(global->line->token[1]) == -1)
	{
		if (global->line->type[1] == TILDE)
		{
			define_home_folder(global);
			if (global->line->cmds->home_folder == NULL)
				ft_printf("minishell: cd: HOME not set\n");
			else if (chdir(global->line->cmds->home_folder) == -1)
				ft_printf("minishell: cd: %s: %s\n",
					global->line->cmds->home_folder, strerror(errno));
		}
		else
			ft_printf("minishell: cd: %s: %s\n", global->line->token[1],
				strerror(errno));
	}
}
