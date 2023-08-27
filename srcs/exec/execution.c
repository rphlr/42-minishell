/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:57:29 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/27 15:50:37 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**env_to_char(t_global *global)
{
	char	**paths;

	if (!global->env)
		return (NULL);
	while (ft_strcmp(global->env->name, "PATH"))
	{
		global->env = global->env->next;
		if (!global->env)
			return (NULL);
	}
	paths = ft_gc_malloc(sizeof(char *) * (ft_strlen(global->env->value) + 1));
	if (!paths)
		return (NULL);
	paths = ft_split(global->env->value, ':');
	return (paths);
}

static char	*get_path(char *command, char **paths)
{
	char	*path;
	char    *tmp;

	if (command[0] == '.' || command[0] == '/')
	{
		if (access(command, F_OK) == 0)
			return (command);
		return (NULL);
	}
	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		path = ft_strjoin(tmp, command);
		if (access(path, F_OK) == 0)
			return (path);
		paths++;
	}
	return (NULL);
}

static int	cmd_is_primaries(char *cmd)
{
	char *cmd_copy = ft_strdup(cmd);
	char *first_word = ft_strtok(cmd_copy, " ");

	if (!ft_strcmp(first_word, "echo"))
		return (1);
	else if (!ft_strcmp(first_word, "cd"))
		return (1);
	else if (!ft_strcmp(first_word, "pwd"))
		return (1);
	else if (!ft_strcmp(first_word, "export"))
		return (1);
	else if (!ft_strcmp(first_word, "unset"))
		return (1);
	else if (!ft_strcmp(first_word, "env"))
		return (1);
	else if (!ft_strcmp(first_word, "exit"))
		return (1);
	return (0);
}

static void	execute_primaries(char	*cmd, t_global *global)
{
	char *cmd_copy = ft_strdup(cmd);
	char *first_word = ft_strtok(cmd_copy, " ");

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
}

void	ft_heredoc(char *filename, char *limiter, int type)
{
	int		file;
	int		fd_final;
	char	*buf;

	fd_final = 0;
	file = open(".heredoc_content", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file < 0)
	{
		ft_printf("Error: failed to open temporary file\n");
		exit (1);
	}
	while (true)
	{
		ft_printf("heredoc> ");
		buf = get_next_line(0);
		if (!buf)
		{
			close(file);
			exit(1);
		}
		if (!ft_strncmp(limiter, buf, ft_strlen(limiter)))
			break ;
		write(file, buf, ft_strlen(buf));
	}
	close(file);
	if (filename)
	{
		if (type == INPUT)
			fd_final = open(filename, O_RDONLY);
		else if (type == APPEND)
			fd_final = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else if (type == OUTPUT)
			fd_final = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd_final < 0)
		{
			ft_printf("minishell: %s: No such file or directory\n", filename);
			exit (1);
		}
		dup2(fd_final, STDOUT_FILENO);
		close(fd_final);
	}
	fd_final = open(".heredoc_content", O_RDONLY);
	if (fd_final < 0)
	{
		unlink(".heredoc_content");
		ft_printf("Error: failed to open temporary file\n");
		exit (1);
	}
	while ((buf = get_next_line(fd_final)))
		write(STDOUT_FILENO, buf, ft_strlen(buf));
	close(fd_final);
	unlink(".heredoc_content");
}

static int execute_cmd(char *cmd, t_redirection *redir, t_global *global)
{
	pid_t pid;
	int status;
	char *argv[100];
	int i = 0;

	char *cmd_ptr = cmd;
    for (int j = 0; global->line->token[j]; j++)
    {
        if (ft_strncmp(cmd_ptr, global->line->token[j], ft_strlen(global->line->token[j])) == 0)
        {
            argv[i] = global->line->token[j];
            i++;
            cmd_ptr += ft_strlen(global->line->token[j]);
            if (*cmd_ptr == ' ')
                cmd_ptr++;
        }
    }
    argv[i] = NULL;
	if (!global->env)
	{
		ft_printf("minishell: %s: No such file or directory\n", argv[0]);
		global->exit_code = 127;
		return (global->exit_code);
	}
	char **paths = env_to_char(global);
	char *path = get_path(argv[0], paths);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", argv[0]);
		global->exit_code = 127;
		return (global->exit_code);
	}
	pid = fork();
	manage_pid(&pid);
	if (!pid)
	{
		if (redir)
		{
			int fd;
			switch (redir->type)
			{
				case HEREDOC_REDIRECTION:
					ft_heredoc(redir->filename, redir->limiter, redir->type_hd);
					exit (EXIT_SUCCESS);
					break;
				case OUTPUT_REDIRECTION:
					fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (fd == -1)
					{
						ft_printf("minishell: %s: %s\n", redir->filename, strerror(errno));
						global->exit_code = 1;
						exit (EXIT_FAILURE);
					}
					dup2(fd, STDOUT_FILENO);
					close(fd);
					break;
				case APPEND_REDIRECTION:
					fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
					if (fd == -1)
					{
						ft_printf("minishell: %s: %s\n", redir->filename, strerror(errno));
						global->exit_code = 1;
						exit(EXIT_FAILURE);
					}
					dup2(fd, STDOUT_FILENO);
					close(fd);
					break;
				case INPUT_REDIRECTION:
					fd = open(redir->filename, O_RDONLY);
					if (fd == -1)
					{
						ft_printf("minishell: %s: %s\n", redir->filename, strerror(errno));
						global->exit_code = 1;
						exit(EXIT_FAILURE);
					}
					dup2(fd, STDIN_FILENO);
					close(fd);
					break;
				case NO_REDIRECTION:
					break;
			}
		}
		execve(path, argv, NULL);
		perror("execve");
		global->exit_code = EXIT_FAILURE;
		// manage_exit(&global->exit_code);
		// manage_pid(&pid);
		exit (global->exit_code);
	}
	else if (pid < 0)
	{
		perror("fork");
		return -1;
	}
	else
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
}

static void ft_redir(t_global *global, t_cmds *curr_cmd)
{
	execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
}

static void ft_or(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	if (execute_cmd(curr_cmd->cmd, curr_cmd->redir, global) != 0)
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
}

static void	ft_and(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	if (execute_cmd(curr_cmd->cmd, curr_cmd->redir, global) == 0)
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
}

static void execute_pipeline(t_global *global, t_cmds *cmds)
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

static void	ft_pipe(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	int fds[2];
	if (pipe(fds) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid_t pid = fork();
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
	
	pid_t pid2 = fork();
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
	close(fds[0]);
	close(fds[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
}


static void	ft_semicolon(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
	execute_cmd(next_cmd->cmd, next_cmd->redir, global);
}

static void	execute_specials(t_global *global)
{
	t_count	*count_tmp;
	t_cmds 	*curr_cmd;
	t_token	*type_tmp;

	count_tmp = global->line->count;
	curr_cmd = global->line->cmds;
	type_tmp = global->line->type;
	while (count_tmp->special_cases)
	{
		while (!(*type_tmp == INPUT || *type_tmp == OUTPUT || *type_tmp == APPEND || *type_tmp == HEREDOC || *type_tmp == AND || *type_tmp == OR || *type_tmp == SEMICOLON || *type_tmp == PIPE) && *type_tmp != END)
			type_tmp++;
		if (*type_tmp == INPUT || *type_tmp == HEREDOC || *type_tmp == APPEND || *type_tmp == OUTPUT)
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
			return ;
		}
		else if (count_tmp->nbr_ors > 0 && *type_tmp == OR)
		{
			count_tmp->nbr_ors--;
			ft_or(global, curr_cmd, curr_cmd->next);
			if (global->exit_code == 0)
				return;
			if (curr_cmd->next)
			{
				curr_cmd = curr_cmd->next;
				type_tmp++;
				while (*type_tmp != OR && *type_tmp != END)
					type_tmp++;
				if (*type_tmp == END)
					return;
			}
		}
		else if (count_tmp->nbr_ands > 0 && *type_tmp == AND)
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
					return;
			}
		}
		else if (count_tmp->nbr_semicolons > 0 && *type_tmp == SEMICOLON)
		{
			count_tmp->nbr_semicolons--;
			ft_semicolon(global, curr_cmd, curr_cmd->next);
		}
		else if (count_tmp->nbr_pipes > 0 && *type_tmp == PIPE)
		{
			if (count_tmp->nbr_pipes > 1)
				execute_pipeline(global, curr_cmd);
			else
				ft_pipe(global, curr_cmd, curr_cmd->next);
			count_tmp->nbr_pipes = 0;
		}
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

void	run_cmd(t_global *global)
{
	int primaries;

	global->exit_code = 0;
	manage_exit(&global->exit_code);
	if (global->line->count->special_cases == true)
	{
		execute_specials(global);
		return ;
	}
	primaries = cmd_is_primaries(global->line->cmds->cmd);
	if (primaries)
	{
		execute_primaries(global->line->cmds->cmd, global);
		return ;
	}
	if (!get_env_value("PATH", global->env))
	{
		ft_printf("minishell: %s: No such file or directory\n", global->line->cmds->cmd);
		global->exit_code = 127;
		manage_exit(&global->exit_code);
		return ;
	}
	global->exit_code = execute_cmd(global->line->cmds->cmd, global->line->cmds->redir, global);
	manage_exit(&global->exit_code);
}
