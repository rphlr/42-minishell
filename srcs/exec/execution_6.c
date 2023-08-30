/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 13:37:24 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/30 15:13:42 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmds(t_cmds *cmds)
{
	int		num_cmds;
	t_cmds	*tmp;

	num_cmds = 0;
	tmp = cmds;
	while (tmp)
	{
		num_cmds++;
		tmp = tmp->next;
	}
	return (num_cmds);
}

void	initialize_pipes(int **fds, int num_cmds, t_global *global)
{
	int	i;

	i = -1;
	while (++i < num_cmds - 1)
	{
		if (pipe(fds[i]) < 0)
		{
			perror("pipe");
			global->exit_code = EXIT_FAILURE;
			exit(global->exit_code);
		}
	}
}

void	dup_and_close(int **fds, int i, int num_cmds)
{
	int	j;

	j = -1;
	if (i != 0)
	{
		dup2(fds[i - 1][0], STDIN_FILENO);
		close(fds[i - 1][0]);
	}
	if (i != num_cmds - 1)
	{
		dup2(fds[i][1], STDOUT_FILENO);
		close(fds[i][1]);
	}
	while (++j < num_cmds - 1)
	{
		if (j != i - 1)
			close(fds[j][0]);
		if (j != i)
			close(fds[j][1]);
	}
}

void	check_first_pid(pid_t pid, int fds[2], t_cmds *curr_cmd,
	t_global *global)
{
	pid = fork();
	if (pid == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
		exit(EXIT_SUCCESS);
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

void	check_second_pid(pid_t pid2, int fds[2], t_cmds *next_cmd,
	t_global *global)
{
	pid2 = fork();
	if (pid2 == 0)
	{
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
		exit(EXIT_SUCCESS);
	}
	else if (pid2 < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}
