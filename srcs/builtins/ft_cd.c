/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:40:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/16 10:29:26 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		ft_printf("minishell: cd: %s: %s\n", global->line->token[1],
			strerror(errno));
}
