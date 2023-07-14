/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:53:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/07/10 10:53:08 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: display prompt when minishell is ready to receive input
// TODO: read input
// TODO: parse input
// TODO: Search and launch the right executable (based on the PATH variable
//		 or by using relative or absolute path)
// TODO: display output
// TODO: loop
// TODO: Have a working history
// TODO: Do not use more than one global variable, think about it and be ready
//		 to explain why you do it.
// TODO: Do not interpret unclosed quotes or unspecified special characters
//		 like \ (eg with $’\n’)
// TODO: Handle ' and " (quotes) correctly
// TODO: Handle redirections > >> < <<
// TODO: Handle pipes | correctly
// TODO: Handle environment variables ($ followed by characters)
// TODO: Handle $? (exit code of the previous program)
// TODO: Handle ctrl-C ctrl-D ctrl-\ correctly
// TODO: Implement echo with option ’-n’
// TODO: Implement cd with only a relative or absolute path
// TODO: Implement pwd without any options
// TODO: Implement export without any options
// TODO: Implement unset without any options
// TODO: Implement env without any options and any arguments
// TODO: Implement exit without any options
// TODO: exit
// BONUS: Implement && and ||
// BONUS: Implement Wilcard * (globbing)

typedef struct s_env
{
	char	**path;
	char	*home;
	char	*pwd;
	char	*oldpwd;
	char	*user;
	char	*shell;
}	t_env;

typedef struct s_cmd
{
	char	**args;
	char	*cmd;
	char	*redir_in;
	char	*redir_out;
	char	*redir_append;
	char	*pipe;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_global
{
	int		exit_code;
	t_env	*env;
	t_cmd	*cmd;
}	t_global;

t_global	*init_env(char **envp)
{
	t_global	*global;
	t_env		*env;
	int			i;

	i = -1;
	global = NULL;
	global = ft_gc_malloc(sizeof(t_global));
	env = ft_gc_malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			env->path = ft_split(envp[i] + 5, ':');
		else if (!ft_strncmp(envp[i], "HOME=", 5))
			env->home = ft_strdup(envp[i] + 5);
		else if (!ft_strncmp(envp[i], "PWD=", 4))
			env->pwd = ft_strdup(envp[i] + 4);
		else if (!ft_strncmp(envp[i], "USER=", 5))
			env->user = ft_strdup(envp[i] + 5);
		else if (!ft_strncmp(envp[i], "SHELL=", 6))
			env->shell = ft_strdup(envp[i] + 6);
		else if (!ft_strncmp(envp[i], "OLDPWD=", 7))
			env->oldpwd = ft_strdup(envp[i] + 7);
	}
	global->env = env;
	global->exit_code = 0;
	return (global);
}

// void	init_cmd(t_global *global)
// {
// 	char	*line;

// 	line = readline(PROMPT);
// 	if (!line)
// 	{
// 		ft_printf("exit\n");
// 		global->exit_code = 1;
// 		return ;
// 	}
// 	if (line[0])
// 		// add_history(line);
// 	// global->cmd = parse_cmd(line);
// 	free(line);
// }

int	main(int ac, char **av, char **envp)
{
	t_global	*global;

	(void)ac;
	(void)av;
	global = init_env(envp);
	if (!global->env)
		return (1);
	// while (!global->exit_code)
	// {
		// init_cmd(global);
		ft_printf(PROMPT);
		ft_printf("command: %s\n", global->cmd->cmd);
	// }
	return (0);
}
