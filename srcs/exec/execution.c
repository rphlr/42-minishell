/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:57:29 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/24 14:01:38 by rrouille         ###   ########.fr       */
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

void	ft_input(t_cmds *curr_cmd)
{
	int fd;

	// Ouvrir le fichier en mode lecture
	fd = open(curr_cmd->input->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("minishell: %s: %s\n", curr_cmd->input->filename, strerror(errno));
		return ;
	}

	// Rediriger l'entrée standard vers le fichier
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		ft_printf("minishell: dup2 error\n");
		return ;
	}

	// Fermer le descripteur de fichier original
	close(fd);
}

void	ft_output(t_cmds *curr_cmd)
{
	int fd;

    printf("0\n");
    fd = open(curr_cmd->output->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    printf("1\n");
    if (fd == -1)
    {
        ft_printf("minishell: %s: %s\n", curr_cmd->output->filename, strerror(errno));
        return ;
    }

    printf("2\n");
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        ft_printf("minishell: dup2 error\n");
        close(fd);  // Assurez-vous de fermer le descripteur de fichier en cas d'erreur
        return ;
    }
    printf("3\n");
    close(fd);
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
		while (!(*type_tmp == INPUT || *type_tmp == OUTPUT || *type_tmp == APPEND || *type_tmp == HEREDOC || *type_tmp == OR || *type_tmp == AND || *type_tmp == SEMICOLON || *type_tmp == PIPE) && *type_tmp != END)
        {
            type_tmp++;
        }
		if (count_tmp->nbr_inputs > 0 && *type_tmp == INPUT)
		{
			count_tmp->nbr_inputs--;
			ft_input(curr_cmd);
		}
		if (count_tmp->nbr_outputs > 0 && *type_tmp == OUTPUT)
		{
			count_tmp->nbr_outputs--;
			ft_output(curr_cmd);
		}
		if (count_tmp->nbr_appends > 0 && *type_tmp == APPEND)
		{
			count_tmp->nbr_appends--;
			// ft_append(global, paths);
		}
		if (count_tmp->nbr_heredocs > 0 && *type_tmp == HEREDOC)
		{
			count_tmp->nbr_heredocs--;
			// ft_heredoc(global, paths);
		}
		if (count_tmp->nbr_ors > 0 && *type_tmp == OR)
		{
			count_tmp->nbr_ors--;
			// ft_or(global, paths);
		}
		if (count_tmp->nbr_ands > 0 && *type_tmp == AND)
		{
			count_tmp->nbr_ands--;
			// ft_and(global, paths);
		}
		if (count_tmp->nbr_semicolons > 0 && *type_tmp == SEMICOLON)
		{
			count_tmp->nbr_semicolons--;
			// ft_semicolon(global, paths);
		}
		if (count_tmp->nbr_pipes > 0 && *type_tmp == PIPE)
		{
			count_tmp->nbr_pipes--;
			// ft_pipe(global, paths);
		}

		curr_cmd = curr_cmd->next;
        type_tmp++;
		if (!count_tmp->nbr_inputs && !count_tmp->nbr_outputs
			&& !count_tmp->nbr_appends && !count_tmp->nbr_heredocs
			&& !count_tmp->nbr_ors && !count_tmp->nbr_ands
			&& !count_tmp->nbr_semicolons && !count_tmp->nbr_pipes)
			count_tmp->special_cases = false;
		// curr_cmd = curr_cmd->next;
		// type_tmp++;
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
            exit(126);  // Utilisez exit pour terminer le processus fils en cas d'erreur
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
}

void	run_cmd(t_global *global)
{
	if (global->line->count->special_cases == true)
	{
		printf("special cases\n");
		execute_specials(global);
		return ;
	}
	if (!ft_strcmp(*global->line->token, "echo"))
		ft_echo(global);
	else if (!ft_strcmp(*global->line->token, "cd"))
		ft_cd(global);
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
	printf("exit code: %d\n", global->exit_code);
	global->exit_code = 0;
}
