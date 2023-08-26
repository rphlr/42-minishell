/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:57:29 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/26 09:19:45 by rrouille         ###   ########.fr       */
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
		ft_echo(cmd);
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

static int execute_cmd(char *cmd, t_redirection *redir, t_global *global)
{
	pid_t pid;
	int status;
	char *argv[100];
	char *token;
	int i = 0;

	token = ft_strtok(cmd, " ");
	while (token != NULL)
	{
		argv[i] = token;
		i++;
		token = ft_strtok(NULL, " ");
	}
	argv[i] = NULL;
	if (!global->env)
	{
		ft_printf("minishell: %s: No such file or directory\n", argv[0]);
		global->exit_code = 127;
		return (manage_exit(&global->exit_code));
	}
	char **paths = env_to_char(global);
	char *path = get_path(argv[0], paths);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", argv[0]);
		global->exit_code = 127;
		return (manage_exit(&global->exit_code));
	}
	pid = fork();
	if (pid == 0)
	{
		if (redir && redir->filename)
		{
			int fd;
			switch (redir->type)
			{
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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		// tcsetattr(0, TCSANOW, &original_term);
		execve(path, argv, NULL);
		global->exit_code = EXIT_FAILURE;
		exit (manage_exit(&global->exit_code));
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
	{
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
	}
}

static void	ft_and(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	if (execute_cmd(curr_cmd->cmd, curr_cmd->redir, global) == 0)
	{
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
	}
}

// static void execute_pipeline(t_global *global, t_cmds *cmds)
// {
//     int num_cmds = 0;
//     t_cmds *tmp = cmds;
//     while (tmp)
//     {
//         num_cmds++;
//         tmp = tmp->next;
//     }

//     int fds[num_cmds - 1][2];
//     for (int i = 0; i < num_cmds - 1; i++)
//     {
//         pipe(fds[i]);
//     }

//     for (int i = 0; i < num_cmds; i++)
//     {
//         pid_t pid = fork();
//         if (pid == 0)
//         {
//             // Si ce n'est pas la première commande, redirigez l'entrée standard depuis le pipe précédent
//             if (i != 0)
//             {
//                 dup2(fds[i - 1][0], STDIN_FILENO);
//             }

//             // Si ce n'est pas la dernière commande, redirigez la sortie standard vers le pipe actuel
//             if (i != num_cmds - 1)
//             {
//                 dup2(fds[i][1], STDOUT_FILENO);
//             }

//             // Fermez tous les descripteurs de fichiers
//             for (int j = 0; j < num_cmds - 1; j++)
//             {
//                 close(fds[j][0]);
//                 close(fds[j][1]);
//             }

//             // Restaurez le comportement par défaut des signaux
//             signal(SIGINT, SIG_DFL);
//             signal(SIGQUIT, SIG_DFL);

//             execute_cmd(cmds->cmd, cmds->redir, global);
//             exit(EXIT_SUCCESS);
//         }

//         cmds = cmds->next;
//     }

//     // Fermez tous les descripteurs de fichiers dans le processus parent
//     for (int i = 0; i < num_cmds - 1; i++)
//     {
//         close(fds[i][0]);
//         close(fds[i][1]);
//     }

//     // Attendez que tous les processus enfants se terminent
//     for (int i = 0; i < num_cmds; i++)
//     {
//         wait(NULL);
//     }
// }

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
    for (int i = 0; i < num_cmds - 1; i++)
    {
        if (pipe(fds[i]) < 0)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_cmds; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            if (i != 0) // Si ce n'est pas la première commande
            {
                dup2(fds[i - 1][0], STDIN_FILENO);
                close(fds[i - 1][0]);
            }
            if (i != num_cmds - 1) // Si ce n'est pas la dernière commande
            {
                dup2(fds[i][1], STDOUT_FILENO);
                close(fds[i][1]);
            }
            
            // Fermeture de tous les descripteurs de fichier inutilisés
            for (int j = 0; j < num_cmds - 1; j++)
            {
                if (j != i - 1) close(fds[j][0]);
                if (j != i) close(fds[j][1]);
            }
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);

            execute_cmd(cmds->cmd, cmds->redir, global);
            exit(EXIT_SUCCESS);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        cmds = cmds->next;
    }
    
    // Fermeture de tous les descripteurs de fichier dans le processus parent
    for (int i = 0; i < num_cmds - 1; i++)
    {
        close(fds[i][0]);
        close(fds[i][1]);
    }
    
    // Attendre que tous les processus fils se terminent
    for (int i = 0; i < num_cmds; i++)
    {
        wait(NULL);
    }
}


void	ft_pipe(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	int		fd[2];
	pid_t	pid1, pid2;

	pipe(fd);
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
		exit(EXIT_SUCCESS);
	}
	else if (pid1 < 0)
	{
		ft_printf("minishell: fork error: %s\n", strerror(errno));
		global->exit_code = 1;
		manage_exit(&global->exit_code);
		return ;
	}

	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
		exit(EXIT_SUCCESS);
	}
	else if (pid2 < 0)
	{
		ft_printf("minishell: fork error: %s\n", strerror(errno));
		global->exit_code = 1;
		manage_exit(&global->exit_code);
		return ;
	}

	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

// static void	ft_pipe(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
// {
// 	int		fd[2];
// 	pid_t	pid;

// 	pipe(fd);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		dup2(fd[1], STDOUT_FILENO);
// 		close(fd[0]);
// 		close(fd[1]);
// 		execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
// 	}
// 	else if (pid < 0)
// 	{
// 		ft_printf("minishell: fork error: %s\n", strerror(errno));
// 		global->exit_code = 1;
// 		manage_exit(&global->exit_code);
// 		return ;
// 	}
// 	else
// 	{
// 		dup2(fd[0], STDIN_FILENO);
// 		close(fd[0]);
// 		close(fd[1]);
// 		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
// 	}
// }

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
		else if (count_tmp->nbr_ors > 0 && *type_tmp == OR)
		{
			count_tmp->nbr_ors--;
			ft_or(global, curr_cmd, curr_cmd->next);
		}
		else if (count_tmp->nbr_ands > 0 && *type_tmp == AND)
		{
			count_tmp->nbr_ands--;
			ft_and(global, curr_cmd, curr_cmd->next);
		}
		else if (count_tmp->nbr_semicolons > 0 && *type_tmp == SEMICOLON)
		{
			count_tmp->nbr_semicolons--;
			ft_semicolon(global, curr_cmd, curr_cmd->next);
		}
		else if (count_tmp->nbr_pipes > 0 && *type_tmp == PIPE)
		{
			count_tmp->nbr_pipes--;
			// ft_pipe(global, curr_cmd, curr_cmd->next);
			// int num_pipes = 0;
			// t_cmds *tmp_cmd = curr_cmd;
			// count_tmp->nbr_pipes--;
			// execute_pipeline(global, curr_cmd);

			// Avancez jusqu'à la fin du pipeline

			// t_token	*tmp = type_tmp;
			// while (*type_tmp != PIPE)
			// {
			// 	curr_cmd = curr_cmd->next;
			// 	type_tmp++;
			// }
			// t_token	*tmp_type = type_tmp;
			// while ((*type_tmp == PIPE) || !((*type_tmp == INPUT || *type_tmp == OUTPUT || *type_tmp == APPEND || *type_tmp == HEREDOC || *type_tmp == OR || *type_tmp == AND || *type_tmp == SEMICOLON) && *type_tmp != END))
			// {
			// 	if (*type_tmp == PIPE)
			// 	{
			// 		num_pipes++;
			// 	}
			// 	type_tmp++;
			// }
			// printf("num_pipes = %d\n", num_pipes);
			// // type_tmp = tmp_type;
			// // Move to the last command in the pipeline
			// for (int i = 0; i <= num_pipes; i++)
			// {
			// 	printf("curr_cmd->cmd = %s\n", curr_cmd->cmd);
			// 	printf("i = %d\n", i);
			// 	tmp_cmd = tmp_cmd->next;
			// }
			
			// execute_pipeline(global, curr_cmd);
			// curr_cmd = tmp_cmd;
			// int num_pipes = 0;
			// t_cmds *tmp_cmd = curr_cmd;

			// while (*type_tmp == PIPE)
			// {
			// 	num_pipes++;
			// 	type_tmp++;
			// }

			// Move type_tmp until next special char or end
			while (!(*type_tmp == INPUT || *type_tmp == OUTPUT || *type_tmp == APPEND ||
					*type_tmp == HEREDOC || *type_tmp == OR || *type_tmp == AND ||
					*type_tmp == SEMICOLON || *type_tmp == END) || *type_tmp == PIPE)
			{
				if (*type_tmp == PIPE)
					count_tmp->nbr_pipes--;
				type_tmp++;
			}

			// // Move to the last command in the pipeline
			// for (int i = 0; i < num_pipes; i++)
			// {
			// 	if (tmp_cmd)
			// 		tmp_cmd = tmp_cmd->next;
			// }
			printf("curr_cmd->cmd = %s\n", curr_cmd->cmd);
			execute_pipeline(global, curr_cmd);
			// curr_cmd = tmp_cmd;
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

void	run_cmd(t_global *global)
{
	int primaries;

	primaries = cmd_is_primaries(global->line->cmds->cmd);
	if (primaries)
	{
		execute_primaries(global->line->cmds->cmd, global);
		return ;
	}
	else if (global->line->count->special_cases == true)
	{
		execute_specials(global);
		return ;
	}
	global->exit_code = execute_cmd(global->line->cmds->cmd, global->line->cmds->redir, global);
	manage_exit(&global->exit_code);
}
