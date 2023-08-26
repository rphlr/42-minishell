/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 17:08:38 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/26 15:58:48 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	manage_exit(int *new_code)
{
	static int	exit_code;

	if (new_code)
		exit_code = *new_code;
	return (exit_code);
}

pid_t	manage_pid(pid_t *new_pid)
{
	static pid_t	child_pid;

	// if (child_pid < 0)
	// 	child_pid = 0;
	if (new_pid)
		child_pid = *new_pid;
	return (child_pid);
}
