/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 13:36:45 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/31 08:29:06 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	checking_primaries(t_global *global, int primaries)
{
	if (global->line->count->special_cases == true)
	{
		execute_specials(global);
		return (1);
	}
	primaries = cmd_is_primaries(global->line->cmds->cmd);
	if (primaries)
	{
		execute_primaries(global->line->cmds->cmd, global);
		return (1);
	}
	if (!get_env_value("PATH", global->env))
	{
		ft_printf("minishell: %s: No such file or directory\n",
			global->line->cmds->cmd);
		global->exit_code = 127;
		manage_exit(&global->exit_code);
		return (1);
	}
	return (0);
}

void	create_file(int fd, t_redirection *redir, t_global *global)
{
	fd = -1;
	if (redir->type == OUTPUT_REDIRECTION)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == APPEND_REDIRECTION)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == INPUT_REDIRECTION)
		fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("minishell: %s: %s\n", redir->filename, strerror(errno));
		global->exit_code = 1;
		exit(EXIT_FAILURE);
	}
	if (redir->type == OUTPUT_REDIRECTION || redir->type == APPEND_REDIRECTION)
		dup2(fd, STDOUT_FILENO);
	else if (redir->type == INPUT_REDIRECTION)
		dup2(fd, STDIN_FILENO);
	close (fd);
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
