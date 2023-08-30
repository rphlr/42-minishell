/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 15:12:13 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/30 16:30:44 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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