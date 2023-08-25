/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:57:29 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/25 14:35:07 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute(t_global *global);

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

int	cmd_is_primaries(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	else if (!ft_strcmp(cmd, "cd"))
		return (1);
	else if (!ft_strcmp(cmd, "pwd"))
		return (1);
	else if (!ft_strcmp(cmd, "export"))
		return (1);
	else if (!ft_strcmp(cmd, "unset"))
		return (1);
	else if (!ft_strcmp(cmd, "env"))
		return (1);
	else if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

void	execute_primaries(char	*cmd, t_global *global)
{
	ft_printf("cmd: %s\n", cmd);
	if (!ft_strcmp(cmd, "echo"))
		ft_echo(cmd);
	else if (!ft_strcmp(cmd, "cd"))
		ft_cd(cmd, global);
	else if (!ft_strcmp(cmd, "pwd"))
		ft_pwd(global->line);
	else if (!ft_strcmp(cmd, "export"))
		ft_export(global, global->line);
	else if (!ft_strcmp(cmd, "unset"))
		ft_unset(global, global->line);
	else if (!ft_strcmp(cmd, "env"))
		ft_env(global);
	else if (!ft_strcmp(cmd, "exit"))
		ft_exit(global);
}

int execute_cmd(char *cmd, t_redirection *redir, t_global *global)
{
    pid_t pid;
    int status;
    char *argv[100];
    char *token;
    int i = 0;

	int primaries = cmd_is_primaries(cmd);
	if (primaries)
	{
		execute_primaries(cmd, global);
		exit(EXIT_SUCCESS);
	}
    token = ft_strtok(cmd, " ");
    while (token != NULL)
    {
        argv[i] = token;
        i++;
        token = ft_strtok(NULL, " ");
    }
    argv[i] = NULL;

    pid = fork();
    if (pid == 0)
    {
        // Processus enfant
        if (redir && redir->filename)
        {
            int fd;
            switch (redir->type)
            {
                case OUTPUT_REDIRECTION:
                    fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd == -1) exit(EXIT_FAILURE); // handle error
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    break;
                case APPEND_REDIRECTION:
                    fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (fd == -1) exit(EXIT_FAILURE); // handle error
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    break;
                case INPUT_REDIRECTION:
                    fd = open(redir->filename, O_RDONLY);
                    if (fd == -1) exit(EXIT_FAILURE); // handle error
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                    break;
                case NO_REDIRECTION:
                    break;
                // Ajoutez d'autres cas si nécessaire
            }
        }
		
        
        char **paths = env_to_char(global);
        char *path = get_path(argv[0], paths);
        execve(path, argv, NULL);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("fork");
        return -1;
    }
    else
    {
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}

void ft_redir(t_global *global, t_cmds *curr_cmd)
{
    execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
}	


void ft_or(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
    if (execute_cmd(curr_cmd->cmd, curr_cmd->redir, global) != 0)
    {
        execute_cmd(next_cmd->cmd, next_cmd->redir, global);
    }
}

void	ft_and(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	if (execute_cmd(curr_cmd->cmd, curr_cmd->redir, global) == 0)
	{
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
	}
}

void	ft_pipe(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	int		fd[2];
	pid_t	pid;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
	}
	else if (pid < 0)
	{
		ft_printf("minishell: fork error: %s\n", strerror(errno));
		global->exit_code = 1;
		return ;
	}
	else
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
	}
}

void	ft_semicolon(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
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
			// printf("curr_cmd0: %s\n", curr_cmd->cmd);
		while (!(*type_tmp == INPUT || *type_tmp == OUTPUT || *type_tmp == APPEND || *type_tmp == HEREDOC || *type_tmp == OR || *type_tmp == AND || *type_tmp == SEMICOLON || *type_tmp == PIPE) && *type_tmp != END)
        {
            type_tmp++;
        }
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
		}
		if (count_tmp->nbr_ors > 0 && *type_tmp == OR)
		{
			count_tmp->nbr_ors--;
			ft_or(global, curr_cmd, curr_cmd->next);
		}
		if (count_tmp->nbr_ands > 0 && *type_tmp == AND)
		{
			count_tmp->nbr_ands--;
			ft_and(global, curr_cmd, curr_cmd->next);
		}
			// printf("curr_cmd1: %s\n", curr_cmd->cmd);
		if (count_tmp->nbr_semicolons > 0 && *type_tmp == SEMICOLON)
		{
			count_tmp->nbr_semicolons--;
			// printf("curr_cmd2: %s\n", curr_cmd->cmd);
			// printf("next_cmd2: %s\n", curr_cmd->next->cmd);
			ft_semicolon(global, curr_cmd, curr_cmd->next);
		}
		if (count_tmp->nbr_pipes > 0 && *type_tmp == PIPE)
		{
			count_tmp->nbr_pipes--;
			ft_pipe(global, curr_cmd, curr_cmd->next);
		}
		curr_cmd = curr_cmd->next;
        type_tmp++;
		if (!count_tmp->nbr_inputs && !count_tmp->nbr_outputs
			&& !count_tmp->nbr_appends && !count_tmp->nbr_heredocs
			&& !count_tmp->nbr_ors && !count_tmp->nbr_ands
			&& !count_tmp->nbr_semicolons && !count_tmp->nbr_pipes)
			count_tmp->special_cases = false;
	}
}

static void	pid_working(char *path, char **paths, t_global *global)
{
    pid_t	pid;

    pid = fork();
    if (pid < 0)
    {
        ft_printf("minishell: fork error: %s\n", strerror(errno));
        global->exit_code = 1;
        return;
    }
    if (!pid)
    {
        if (execve(path, global->line->token, paths) == -1)
        {
            ft_printf("minishell: %s: %s\n", *global->line->token, strerror(errno));
            exit(126);
        }
    }
    else
    {
        waitpid(pid, &global->exit_code, 0);
        global->exit_code = WEXITSTATUS(global->exit_code);
    }
	// pid_t	pid;

	// pid = fork();
	// if (!pid)
	// {
	// 	if (execve(path, global->line->token, paths) == -1)
	// 	{
	// 		ft_printf("minishell: %s: %s\n", *global->line->token,
	// 			strerror(errno));
	// 		global->exit_code = 126;
	// 	}
	// }
	// else if (pid == -1)
	// {
	// 	ft_printf("minishell: %s: %s\n", *global->line->token, strerror(errno));
	// 	global->exit_code = 126;
	// }
	// else
	// {
	// 	waitpid(pid, &global->exit_code, 0);
	// 	global->exit_code = WEXITSTATUS(global->exit_code);
	// }
}

void	execute(t_global *global)
{
	char	*path;
	char	**paths;

	paths = env_to_char(global);
	if (!paths)
	{
		ft_printf("minishell: %s: error while loading the path variable\n", *global->line->token);
		global->exit_code = 127;
		return ;
	}
	path = get_path(*global->line->token, paths);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", *global->line->token);
		global->exit_code = 127;
		return ;
	}
	if (global->line->count->special_cases == true)
	{
		execute_specials(global);
		return ;
	}
	pid_working(path, paths, global);
	// execute_cmd(global->line->cmds->cmd, global->line->cmds->redir, global);
}

void	run_cmd(t_global *global)
{
	if (global->line->count->special_cases == true)
	{
		execute_specials(global);
		return ;
	}
	if (!ft_strcmp(*global->line->token, "echo"))
		ft_echo(global->line->cmds->cmd);
	else if (!ft_strcmp(*global->line->token, "cd"))
		ft_cd(global->line->cmds->cmd, global);
	else if (!ft_strcmp(*global->line->token, "pwd"))
		ft_pwd(global->line);
	else if (!ft_strcmp(*global->line->token, "export"))
		ft_export(global, global->line);
	else if (!ft_strcmp(*global->line->token, "unset"))
		ft_unset(global, global->line);
	else if (!ft_strcmp(*global->line->token, "env"))
		ft_env(global);
	else if (!ft_strcmp(*global->line->token, "exit"))
		ft_exit(global);
	else
	{
		execute(global);
		return ;
	}
	global->exit_code = 0;
}
