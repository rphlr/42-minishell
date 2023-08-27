/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:40:18 by rrouille          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/08/27 15:34:31 by mariavillar      ###   ########.fr       */
=======
/*   Updated: 2023/08/25 13:49:02 by rrouille         ###   ########.fr       */
>>>>>>> 70258eb3ff850f618045b294be1b488fbfdff991
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(char *cmd, t_global *global)
{
	char	*path;

	char **token = ft_split(cmd, ' ');
	if (token[1] == NULL)
	{
		path = get_env_value("HOME", global->env);
		if (path == NULL)
			ft_printf("minishell: cd: HOME not set\n");
		else if (chdir(path) == -1)
			ft_printf("minishell: cd: %s: %s\n", path, strerror(errno));
	}
	else if (token[2] != NULL)
		ft_printf("minishell: cd: too many arguments\n");
	else if (chdir(token[1]) == -1)
	{
<<<<<<< HEAD
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
=======
		ft_printf("minishell: cd: %s: %s\n", token[1],
		strerror(errno));
>>>>>>> 70258eb3ff850f618045b294be1b488fbfdff991
	}
}
