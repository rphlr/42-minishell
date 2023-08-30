/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 17:41:09 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/30 18:05:25 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_pipe(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	int		fds[2];
	pid_t	pid;
	pid_t	pid2;

	pid = 0;
	pid2 = 0;
	if (pipe(fds) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	check_first_pid(pid, fds, curr_cmd, global);
	check_second_pid(pid2, fds, next_cmd, global);
	close(fds[0]);
	close(fds[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
}

void	execute_pipeline(t_global *global, t_cmds *cmds)
{
	int		num_cmds;
	int		i;
	pid_t	pid;
	int		**fds;

	num_cmds = count_cmds(cmds);
	fds = ft_gc_malloc(sizeof(int *) * (num_cmds - 1));
	if (!fds)
		exit(EXIT_FAILURE);
	i = -1;
	while (++i < num_cmds - 1)
	{
		fds[i] = ft_gc_malloc(sizeof(int) * 2);
		if (!fds[i])
			exit(EXIT_FAILURE);
	}
	i = -1;
	num_cmds = count_cmds(cmds);
	initialize_pipes(fds, num_cmds, global);
	while (++i < num_cmds)
	{
		pid = fork();
		if (pid == 0)
		{
			dup_and_close(fds, i, num_cmds);
			execute_cmd(cmds->cmd, cmds->redir, global);
			global->exit_code = EXIT_SUCCESS;
			exit(global->exit_code);
		}
		else if (pid < 0)
		{
			perror("fork");
			global->exit_code = EXIT_FAILURE;
			exit(global->exit_code);
		}
		cmds = cmds->next;
	}
	i = -1;
	while (++i < num_cmds - 1)
	{
		close(fds[i][0]);
		close(fds[i][1]);
	}
	i = -1;
	while (++i < num_cmds)
		wait(NULL);
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
