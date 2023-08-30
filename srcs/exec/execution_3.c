/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 16:16:17 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/30 16:36:14 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_heredoc(char *filename, char *limiter, int type)
{
	create_heredoc_file();
	process_heredoc(limiter);
	handle_redirection(filename, type);
	g_current_state = STATE_NORMAL;
}

char	*found_command(t_global *global, char **paths, char *argv[])
{
	char	*path;

	path = get_path(argv[0], paths);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", argv[0]);
		global->exit_code = 127;
		manage_exit(&global->exit_code);
	}
	return (path);
}

int	pid_creation(t_global *global, char **paths, char *argv[],
		t_redirection *redir)
{
	pid_t	pid;
	int		status;

	g_current_state = STATE_BLOCKING_CMD;
	pid = fork();
	manage_pid(&pid);
	if (!pid)
	{
		if (handle_redirections(redir, global))
			return (manage_exit(NULL));
		printf("jsadfjfasdjhg");
		execve(found_command(global, paths, argv), argv, NULL);
		if (manage_exit(NULL) == 127)
			exit(127);
		global->exit_code = EXIT_FAILURE;
		exit(global->exit_code);
	}
	else if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	waitpid(pid, &status, 0);
	g_current_state = STATE_NORMAL;
	if (manage_exit(NULL) != 130)
		return (WEXITSTATUS(status));
	return (manage_exit(NULL));
}

void	fill_argv(char *cmd, char **argv, t_global *global)
{
	int		i;
	int		j;
	char	*cmd_ptr;

	i = 0;
	cmd_ptr = cmd;
	j = -1;
	while (global->line->token[++j])
	{
		if (ft_strncmp(cmd_ptr, global->line->token[j],
				ft_strlen(global->line->token[j])) == 0)
		{
			argv[i] = global->line->token[j];
			i++;
			cmd_ptr += ft_strlen(global->line->token[j]);
			if (*cmd_ptr == ' ')
				cmd_ptr++;
		}
	}
	argv[i] = NULL;
}

int	handle_redirections(t_redirection *redir, t_global *global)
{
	int	fd;

	fd = 0;
	if (!redir)
		return (0);
	if (redir->type == HEREDOC_REDIRECTION)
	{
		ft_heredoc(redir->filename, redir->limiter, redir->type_hd);
		exit(EXIT_SUCCESS);
	}
	create_file(fd, redir, global);
	return (1);
}
