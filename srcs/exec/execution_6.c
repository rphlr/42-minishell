/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 13:37:24 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/30 14:52:45 by mariavillar      ###   ########.fr       */
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
// ----------------------------------------

int	check_or(t_count *count_tmp, t_global *global, t_cmds *curr_cmd,
	t_token *type_tmp)
{
	if (count_tmp->nbr_ors > 0 && *type_tmp == OR)
		{
			count_tmp->nbr_ors--;
			ft_or(global, curr_cmd, curr_cmd->next);
			if (global->exit_code == 0)
				return (1);
			if (curr_cmd->next)
			{
				curr_cmd = curr_cmd->next;
				type_tmp++;
				while (*type_tmp != OR && *type_tmp != END)
					type_tmp++;
				if (*type_tmp == END)
					return (1);
			}
			return (1);
		}
		return (0);
}

int	check_and(t_count *count_tmp, t_global *global, t_cmds *curr_cmd,
	t_token *type_tmp)
{
	if (count_tmp->nbr_ands > 0 && *type_tmp == AND)
		{
			count_tmp->nbr_ands--;
			ft_and(global, curr_cmd, curr_cmd->next);
			if (curr_cmd->next)
			{
				curr_cmd = curr_cmd->next;
				type_tmp++;
				while (*type_tmp != AND && *type_tmp != END)
					type_tmp++;
				if (*type_tmp == END)
					return (1);
			}
			return (1);
		}
		return (0);
}

int	check_semicolon_and_pipe(t_count *count_tmp, t_global *global, t_cmds *curr_cmd,
	t_token *type_tmp)
{
	if (count_tmp->nbr_semicolons > 0 && *type_tmp == SEMICOLON)
		{
			count_tmp->nbr_semicolons--;
			ft_semicolon(global, curr_cmd, curr_cmd->next);
			return (1);
		}
		else if (count_tmp->nbr_pipes > 0 && *type_tmp == PIPE)
		{
			if (count_tmp->nbr_pipes > 1)
				execute_pipeline(global, curr_cmd);
			else
				ft_pipe(global, curr_cmd, curr_cmd->next);
			count_tmp->nbr_pipes = 0;
			return (1);
		}
	return (0);
}

int	check_special(t_count *count_tmp, t_global *global, t_cmds *curr_cmd,
	t_token *type_tmp)
{
	if (*type_tmp == INPUT || *type_tmp == HEREDOC || *type_tmp == APPEND
			|| *type_tmp == OUTPUT)
		{
			if (count_tmp->nbr_inputs > 0 && *type_tmp == INPUT)
				count_tmp->nbr_inputs--;
			else if (count_tmp->nbr_heredocs > 0 && *type_tmp == HEREDOC)
				count_tmp->nbr_heredocs--;
			else if (count_tmp->nbr_appends > 0 && *type_tmp == APPEND)
				count_tmp->nbr_appends--;
			else if (count_tmp->nbr_outputs > 0 && *type_tmp == OUTPUT)
				count_tmp->nbr_outputs--;
			ft_redir(global, curr_cmd);
			return (1);
		}
		return (0);
}

void	call_checks(t_count *count_tmp, t_global *global, t_cmds *curr_cmd,
	t_token *type_tmp)
{
	if (check_special(count_tmp, global, curr_cmd, type_tmp))
		return ;
	else if (check_or(count_tmp, global, curr_cmd, type_tmp))
		return ;
	else if (check_and(count_tmp, global, curr_cmd, type_tmp))
		return ;
	else if (check_semicolon_and_pipe(count_tmp, global, curr_cmd, type_tmp))
		return ;
}

void	execute_specials(t_global *global)
{
	t_count	*count_tmp;
	t_cmds	*curr_cmd;
	t_token	*type_tmp;

	count_tmp = global->line->count;
	curr_cmd = global->line->cmds;
	type_tmp = global->line->type;
	while (count_tmp->special_cases)
	{
		while (!(*type_tmp == INPUT || *type_tmp == OUTPUT || *type_tmp
				== APPEND || *type_tmp == HEREDOC || *type_tmp == AND
				|| *type_tmp == OR || *type_tmp == SEMICOLON || *type_tmp
				== PIPE) && *type_tmp != END)
			type_tmp++;
		call_checks(count_tmp, global, curr_cmd, type_tmp);
		if (curr_cmd->next)
			curr_cmd = curr_cmd->next;
		type_tmp++;
		if (!count_tmp->nbr_inputs && !count_tmp->nbr_outputs
			&& !count_tmp->nbr_appends && !count_tmp->nbr_heredocs
			&& !count_tmp->nbr_ors && !count_tmp->nbr_ands
			&& !count_tmp->nbr_semicolons && !count_tmp->nbr_pipes)
			count_tmp->special_cases = false;
	}
}
