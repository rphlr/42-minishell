/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:40:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/25 13:49:02 by rrouille         ###   ########.fr       */
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
		ft_printf("minishell: cd: %s: %s\n", token[1],
		strerror(errno));
	}
}
