/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:42:00 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/13 11:44:49 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_global *global)
{
	t_env	*tmp;

	tmp = global->env;
	while (tmp)
	{
		printf("%s=%s\n", tmp->name, tmp->value);
		// printf("Value: %s\n", tmp->value);
		tmp = tmp->next;
	}
	// if (global->env->user)
	// 	ft_printf("USER=%s\n", global->env->user);
	// if (global->env->path)
	// {
	// 	ft_printf("PATH=");
	// 	if (*global->env->path)
	// 		ft_printf("%s", *global->env->path++);
	// 	while (*global->env->path)
	// 		ft_printf(":%s", *global->env->path++);
	// 	ft_printf("\n");
	// }
	// if (global->env->home)
	// 	ft_printf("HOME=%s\n", global->env->home);
	// if (global->env->shell)
	// 	ft_printf("SHELL=%s\n", global->env->shell);
	// if (global->env->pwd)
	// 	ft_printf("PWD=%s\n", global->env->pwd);
	// if (global->env->oldpwd)
	// 	ft_printf("OLDPWD=%s\n", global->env->oldpwd);
}
