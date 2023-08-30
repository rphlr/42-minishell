/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 17:08:38 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/30 15:59:33 by rrouille         ###   ########.fr       */
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

	if (new_pid)
		child_pid = *new_pid;
	return (child_pid);
}
