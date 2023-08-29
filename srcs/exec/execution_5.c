/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 13:36:45 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/29 18:56:27 by mariavillar      ###   ########.fr       */
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

// --------------------------------------------------

//static int	count_cmds(t_cmds *cmds)
//{
//	int		num_cmds = 0;
//	t_cmds	*tmp = cmds;

//	while (tmp)
//	{
//		num_cmds++;
//		tmp = tmp->next;
//	}
//	return (num_cmds);
//}

//static void	initialize_pipes(int fds[][2], int num_cmds, t_global *global)
//{
//	int	i = -1;

//	while (++i < num_cmds - 1)
//	{
//		if (pipe(fds[i]) < 0)
//		{
//			perror("pipe");
//			global->exit_code = EXIT_FAILURE;
//			exit(global->exit_code);
//		}
//	}
//}

//static void	dup_and_close(int fds[][2], int i, int num_cmds)
//{
//	int	j = -1;

//	if (i != 0)
//	{
//		dup2(fds[i - 1][0], STDIN_FILENO);
//		close(fds[i - 1][0]);
//	}
//	if (i != num_cmds - 1)
//	{
//		dup2(fds[i][1], STDOUT_FILENO);
//		close(fds[i][1]);
//	}
//	while (++j < num_cmds - 1)
//	{
//		if (j != i - 1)
//			close(fds[j][0]);
//		if (j != i)
//			close(fds[j][1]);
//	}
//}

//void	execute_pipeline(t_global *global, t_cmds *cmds)
//{
//	int		num_cmds;
//	int		i = -1;
//	pid_t	pid;
//	int		fds[count_cmds(cmds) - 1][2];

//	num_cmds = count_cmds(cmds);
//	initialize_pipes(fds, num_cmds, global);
//	while (++i < num_cmds)
//	{
//		pid = fork();
//		if (pid == 0)
//		{
//			dup_and_close(fds, i, num_cmds);
//			execute_cmd(cmds->cmd, cmds->redir, global);
//			global->exit_code = EXIT_SUCCESS;
//			exit(global->exit_code);
//		}
//		else if (pid < 0)
//		{
//			perror("fork");
//			global->exit_code = EXIT_FAILURE;
//			exit(global->exit_code);
//		}
//		cmds = cmds->next;
//	}
//	i = -1;
//	while (++i < num_cmds - 1)
//	{
//		close(fds[i][0]);
//		close(fds[i][1]);
//	}
//	i = -1;
//	while (++i < num_cmds)
//		wait(NULL);
//}

void	execute_pipeline(t_global *global, t_cmds *cmds)
{
	{
	int num_cmds = 0;
	t_cmds *tmp = cmds;
	while (tmp)
	{
		num_cmds++;
		tmp = tmp->next;
	}
	int fds[num_cmds - 1][2];
	int i = -1;
	while (++i < num_cmds - 1)
	{
		if (pipe(fds[i]) < 0)
		{
			perror("pipe");
			global->exit_code = EXIT_FAILURE;
			exit (global->exit_code);
		}
	}
	i = -1;
	while (++i < num_cmds)
	{
		pid_t pid = fork();
		if (!pid)
		{
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
			int j = -1;
			while (++j < num_cmds - 1)
			{
				if (j != i - 1) close(fds[j][0]);
				if (j != i) close(fds[j][1]);
			}
			execute_cmd(cmds->cmd, cmds->redir, global);
			global->exit_code = EXIT_SUCCESS;
			exit (global->exit_code);
		}
		else if (pid < 0)
		{
			perror("fork");
			global->exit_code = EXIT_FAILURE;
			exit (global->exit_code);
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
}
