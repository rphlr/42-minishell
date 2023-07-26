/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:53:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/07/26 09:13:15 by rrouille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

// ✅: display prompt when minishell is ready to receive input
// ✅: read input
// ❌: parse input
// ❌: Search and launch the right executable (based on the PATH variable
//		 or by using relative or absolute path)
// ❌: display output
// ✅: loop
// ✅: Have a working history
// ❌: Do not use more than one global variable, think about it and be ready
//		 to explain why you do it.
// ❌: Do not interpret unclosed quotes or unspecified special characters
//		 like \ (eg with $’\n’)
// ❌: Handle ' and " (quotes) correctly
// ❌: Handle redirections > >> < <<
// ❌: Handle pipes | correctly
// ❌: Handle environment variables ($ followed by characters)
// ❌: Handle $? (exit code of the previous program)
// ❌: Handle ctrl-C ctrl-D ctrl-\ correctly
// ❌: Implement echo with option ’-n’
// ❌: Implement cd with only a relative or absolute path
// ❌: Implement pwd without any options
// ❌: Implement export without any options
// ❌: Implement unset without any options
// ❌: Implement env without any options and any arguments
// ✅: Implement exit without any options
// ✅: exit
// BONUS: Implement && and ||
// BONUS: Implement Wilcard * (globbing)

t_env	*init_env(char **envp)
{
	t_env	*env;
	int		i;

	i = -1;
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
	return (env);
}

t_cmd	*init_cmds(char **tokens)
{
	t_cmd	*cmd;

	cmd = ft_gc_malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->token = tokens;
	cmd->type = init_tokens_type(tokens);
	cmd->cmd = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->pipe = NULL;
	cmd->next = NULL;
	return (cmd);
}

bool	check_token(char *line)
{
	if (!line)
	{
		free(line);
		ft_printf("Error while loading argument, exiting...\n");
		return false;
	}
	return true;
}

bool	check_exit_token(char *arg)
{
	int	i;

	i = -1;
	while (arg[++i])
	{
		if (!ft_isdigit(arg[i]))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", arg);
			return (false);
		}
	}
	return (true);
}

int parse_cmd(t_cmd* cmd)
{
	int i;
	
	i = 0;
	while (cmd->token[i])
	{
		if (!ft_strcmp(cmd->token[i], "$HOME"))
		{
			cmd->token[i] = cmd->env->home;
		}
		else if (!ft_strcmp(cmd->token[i], "$PWD"))
		{
			cmd->token[i] = cmd->env->pwd;
		}
		else if (!ft_strcmp(cmd->token[i], "$OLDPWD"))
		{
			cmd->token[i] = cmd->env->oldpwd;
		}
		else if (!ft_strcmp(cmd->token[i], "$USER"))
		{
			cmd->token[i] = cmd->env->user;
		}
		else if (!ft_strcmp(cmd->token[i], "$SHELL"))
		{
			cmd->token[i] = cmd->env->shell;
		}
		else if (!ft_strcmp(cmd->token[i], "$?"))
		{
			cmd->token[i] = ft_itoa(cmd->global->exit_code);
		}
		else if (!ft_strcmp(cmd->token[i], "<"))
		{
			
		}
		else if (!ft_strcmp(cmd->token[i], "<<"))
		{

		}
		else if (!ft_strcmp(cmd->token[i], ">"))
		{

		}
		else if (!ft_strcmp(cmd->token[i], ">"))
		{

		}
		else if (!ft_strcmp(cmd->token[i], ">>"))
		{

		}
		else if (!ft_strcmp(cmd->token[i], "|"))
		{

		}
		i++;
	}
	return 0;
}

void	execute(t_cmd *cmd)
{
	(void) cmd;
}

char	*epur_str(char *line)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (line[++i])
	{
		if (i == 0)
		{
			while (line[i] == ' ' || line[i] == '\t')
					i++;
		}
		if (line[i] == ' ' || line[i] == '\t')
		{
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (!line[i])
				break ;
			line[j++] = ' ';
		}
		line[j++] = line[i];
	}
	line[j] = '\0';
	return (line);
}

int	ft_strstart(char *str, char *start)
{
	int	i;

	i = -1;
	while (str[++i] && start[i])
	{
		if (str[i] != start[i])
			return (0);
	}
	return (1);
}

void	ft_cd(t_cmd *cmd)
{
	(void) cmd;
}

void	ft_pwd(t_cmd *cmd)
{
	(void) cmd;
}

void	ft_export(t_cmd *cmd)
{
	(void) cmd;
}

void	ft_unset(t_cmd *cmd)
{
	(void) cmd;
}

void	ft_env(t_cmd *cmd)
{
	(void) cmd;
}

void	run_cmd(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->token[0], "echo"))
	{
		printf("echo\n");
		ft_echo(cmd);
	}
	else if (!ft_strcmp(cmd->token[0], "cd"))
		ft_cd(cmd);
	else if (!ft_strcmp(cmd->token[0], "pwd"))
		ft_pwd(cmd);
	else if (!ft_strcmp(cmd->token[0], "export"))
		ft_export(cmd);
	else if (!ft_strcmp(cmd->token[0], "unset"))
		ft_unset(cmd);
	else if (!ft_strcmp(cmd->token[0], "env"))
		ft_env(cmd);
	else
		execute(cmd);
}

int	lsh_loop(void)
{
	char	*line;
	t_cmd	*cmd;
	int		err_code;

	err_code = 0;
	while (1)
	{
		line = readline(PROMPT);
		if (!check_token(line))
			break ;
		if (line && ft_strcmp(line, ""))
			add_history(line);
		line = epur_str(line);
		if (!ft_strcmp(line, ""))
			continue ;
		cmd = init_cmds(ft_split(line, ' '));
		free(line);
		parse_cmd(cmd);
		if (!ft_strcmp(cmd->token[0], "exit"))
		{
			ft_printf("exit\n");
			if (!cmd->token[1])
				break ;
			if (!check_exit_token(cmd->token[1]))
				err_code = 255;
			else if (cmd->token[2])
			{
				ft_printf("minishell: exit: too many arguments\n");
				continue ;
			}
			else if (cmd->token[1])
			{
				err_code = ft_atoi(cmd->token[1]);
			 	if (err_code < 0 || err_code > 255)
					err_code %= 256;
			}
			break ;
		}
		run_cmd(cmd);
	}
	return (err_code);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;

	(void)ac;
	(void)av;
	env = init_env(envp);
	if (!env)
		return (1);
	int err_code = lsh_loop();
	exit (err_code);
}