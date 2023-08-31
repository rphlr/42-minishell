/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 01:21:32 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/31 08:46:42 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	is_filename(char *filename, int type)
{
	int		fd_final;
	int		flags;

	fd_final = 0;
	flags = 0;
	if (filename)
	{
		flags = O_CREAT | O_WRONLY;
		if (type == APPEND)
			flags |= O_APPEND;
		else if (type == OUTPUT)
			flags |= O_TRUNC;
		fd_final = open_and_check(filename, flags);
		dup2(fd_final, STDOUT_FILENO);
		close(fd_final);
	}
}

void	handle_redirection(char *filename, int type)
{
	int		fd_final;
	char	*buf;

	is_filename(filename, type);
	fd_final = open_and_check(".heredoc_content", O_RDONLY);
	buf = get_next_line(fd_final);
	while (buf)
	{
		write(STDOUT_FILENO, buf, ft_strlen(buf));
		buf = get_next_line(fd_final);
	}
	close(fd_final);
	unlink(".heredoc_content");
}

int	cmd_is_primaries(char *cmd)
{
	char	*cmd_copy;
	char	*first_word;

	cmd_copy = ft_strdup(cmd);
	first_word = ft_strtok(cmd_copy, " ");
	if (!ft_strcmp(first_word, "echo") || !ft_strcmp(first_word, "cd")
		|| !ft_strcmp(first_word, "pwd") || !ft_strcmp(first_word, "export")
		|| !ft_strcmp(first_word, "unset") || !ft_strcmp(first_word, "env")
		|| !ft_strcmp(first_word, "exit") || !ft_strcmp(first_word, "clear")
		|| !ft_strcmp(first_word, "easter_egg") || !ft_strcmp(first_word, "ee")
		|| !ft_strcmp(first_word, "ms"))
		return (1);
	return (0);
}

void	execute_primaries(char	*cmd, t_global *global)
{
	char	*cmd_copy;
	char	*first_word;

	cmd_copy = ft_strdup(cmd);
	first_word = ft_strtok(cmd_copy, " ");
	if (!ft_strcmp(first_word, "echo"))
		ft_echo(cmd, global);
	else if (!ft_strcmp(first_word, "cd"))
		ft_cd(cmd, global);
	else if (!ft_strcmp(first_word, "pwd"))
		ft_pwd(global->line);
	else if (!ft_strcmp(first_word, "export"))
		ft_export(global, global->line);
	else if (!ft_strcmp(first_word, "unset"))
		ft_unset(global, global->line);
	else if (!ft_strcmp(first_word, "env"))
		ft_env(global);
	else if (!ft_strcmp(first_word, "exit"))
		ft_exit(global);
	else if (!ft_strcmp(first_word, "clear"))
		ft_printf(C_CLEAR);
	else if (!ft_strcmp(first_word, "easter_egg") || !ft_strcmp(first_word, "ee"))
		ft_easter_egg();
	else if (!ft_strcmp(first_word, "ms"))
		ft_mslvl_see(global->env);
}

int	execute_cmd(char *cmd, t_redirection *redir, t_global *global)
{
	char	*argv[100];
	char	**paths;

	fill_argv(cmd, argv, global);
	if (!global->env)
	{
		ft_printf("minishell: %s: No such file or directory\n", argv[0]);
		global->exit_code = 127;
		return (global->exit_code);
	}
	paths = env_to_char(global);
	return (pid_creation(global, paths, argv, redir));
}
