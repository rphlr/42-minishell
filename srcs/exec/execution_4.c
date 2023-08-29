/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 13:35:39 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/29 18:10:13 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_redir(t_global *global, t_cmds *curr_cmd)
{
	execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
}

void	ft_or(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	if (execute_cmd(curr_cmd->cmd, curr_cmd->redir, global) != 0)
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
}

void	ft_and(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	if (execute_cmd(curr_cmd->cmd, curr_cmd->redir, global) == 0)
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
}

void	ft_semicolon(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
	execute_cmd(next_cmd->cmd, next_cmd->redir, global);
}

void	run_cmd(t_global *global)
{
	int	primaries;

	primaries = 0;
	global->exit_code = 0;
	manage_exit(&global->exit_code);
	if (checking_primaries(global, primaries))
		return ;
	global->exit_code = execute_cmd(global->line->cmds->cmd,
			global->line->cmds->redir, global);
	manage_exit(&global->exit_code);
}
