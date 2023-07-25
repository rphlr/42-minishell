/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:53:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/07/16 18:30:27 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_cmd	*init_cmds(char **cmds)
{
	t_cmd	*cmd;

	cmd = ft_gc_malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = cmds;
	cmd->cmd = NULL;
	cmd->redir_in = NULL;
	cmd->redir_out = NULL;
	cmd->redir_append = NULL;
	cmd->pipe = NULL;
	cmd->next = NULL;
	return (cmd);
}

bool	check_args(char *line)
{
	if (!line)
	{
		free(line);
		ft_printf("Error while loading argument, exiting...\n");
		return false;
	}
	return true;
}

bool	check_exit_args(char *arg)
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

int	parse_cmd(t_cmd *cmd)
{
	int	i;

	i = -1;
	while (cmd->args[++i])
	{
		if (!ft_strcmp(cmd->args[i], ">") || !ft_strcmp(cmd->args[i], ">>") || !ft_strcmp(cmd->args[i], "<") || !ft_strcmp(cmd->args[i], "<<"))
		{
			if (!cmd->args[i + 1])
			{
				ft_printf("minishell: syntax error near unexpected token `newline'\n");
				return (1);
			}
			cmd->redir_out = cmd->args[i + 1];
			cmd->args[i] = NULL;
			cmd->args[i + 1] = NULL;
		}
		else if (!ft_strcmp(cmd->args[i], "|") || !ft_strcmp(cmd->args[i], ";") || !ft_strcmp(cmd->args[i], "&&") || !ft_strcmp(cmd->args[i], "||") || !ft_strcmp(cmd->args[i], "!"))
		{
			if (!cmd->args[i + 1])
			{
				ft_printf("minishell: syntax error near unexpected token `%s'\n", cmd->args[i]);
				return (1);
			}
			if (!ft_strcmp(cmd->args[i], "|"))
				cmd->pipe = true;
			cmd->args[i] = NULL;
			cmd->args[i + 1] = NULL;
			break ;
		}
		else if (!ft_strcmp(cmd->args[i], "/") || !ft_strcmp(cmd->args[i], "./") || !ft_strcmp(cmd->args[i], "../"))
		{
			ft_printf("minishell: %s: is a directory\n", cmd->args[i]);
			break ;
		}
	}
	return (0);
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

char	*ignore_quotes(char *str)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			continue ;
		str[j++] = str[i];
	}
	str[j] = '\0';
	return (str);
}

int	check_syntax(char *str)
{
	int	i;
	int	nbr_of_simple_cote;
	int	nbr_of_double_cote;

	i = -1;
	nbr_of_simple_cote = 0;
	nbr_of_double_cote = 0;
	while (str[++i])
	{
		if (str[i] == '\'')
			nbr_of_simple_cote++;
		if (str[i] == '\"')
			nbr_of_double_cote++;
	}
	if (nbr_of_simple_cote % 2 != 0 || nbr_of_double_cote % 2 != 0)
		return (0);
	return (1);
}

int	check_n(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str)
	{
		if (str[j++] == '-' && str[j] == 'n')
		{
			while (str[j] == 'n')
				j++;
			return (1);
		}
	}
	return (0);
}

int	check_e(char **str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[++i])
	{
		if (str[i][j++] == '-' && str[i][j] == 'e')
		{
			while (str[i][j] == 'e')
				j++;
			if (!str[i][j])
				return (1);
		}
	}
	return (0);
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

void	echo_print_special_char(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{

		str = ignore_quotes(str);
		if (str[i] == '\\')
		{
			if (str[i + 1] == 'n')
				ft_printf("\n");
			else if (str[i + 1] == 't')
				ft_printf("\t");
			else if (str[i + 1] == 'v')
				ft_printf("\v");
			else if (str[i + 1] == 'b')
				ft_printf("\b");
			else if (str[i + 1] == 'r')
				ft_printf("\r");
			else if (str[i + 1] == 'f')
				ft_printf("\f");
			else if (str[i + 1] == 'a')
				ft_printf("\a");
			else if (str[i + 1] == '\\')
			{
				if (str[i + 2] == 'n')
					ft_printf("\n");
				else if (str[i + 2] == 't')
					ft_printf("\t");
				else if (str[i + 2] == 'v')
					ft_printf("\v");
				else if (str[i + 2] == 'b')
					ft_printf("\b");
				else if (str[i + 2] == 'r')
					ft_printf("\r");
				else if (str[i + 2] == 'f')
					ft_printf("\f");
				else if (str[i + 2] == 'a')
					ft_printf("\a");
				else if (str[i + 2] == '\\')
					ft_printf("\\");;
				i++;
			}
			else if (str[i + 1] == '0')
				ft_printf("\0");
			else
				ft_printf("%c", str[i + 1]);
			i++;
		}
		else
			ft_printf("%c", str[i]);
	}
}

void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	n;
	int	e;
	int	j;

	i = 0;
	n = 0;
	j = 0;
	e = 0;
	while (cmd->args[++i])
	{
		n = check_n(cmd->args);
		ft_printf("n = %d\n", n);
		e = check_e(cmd->args);
		ft_printf("e = %d\n", e);
		if (!ft_strstart(cmd->args[i], "-n") && !ft_strstart(cmd->args[i], "-e"))
		{
			if (j)
				ft_printf(" ");
			if (!check_syntax(cmd->args[i]))
				return ; // need to add syntax
			else if (e)
				echo_print_special_char(cmd->args[i]);
			else
			{
				cmd->args[i] = ignore_quotes(cmd->args[i]);
				ft_printf("%s", cmd->args[i]);
			}
			j++;
		}
	}
	if (!n)
		ft_printf("\n");
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
	if (!ft_strcmp(cmd->args[0], "echo"))
		ft_echo(cmd);
	else if (!ft_strcmp(cmd->args[0], "cd"))
		ft_cd(cmd);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		ft_pwd(cmd);
	else if (!ft_strcmp(cmd->args[0], "export"))
		ft_export(cmd);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		ft_unset(cmd);
	else if (!ft_strcmp(cmd->args[0], "env"))
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
		if (!check_args(line))
			break ;
		if (line && ft_strcmp(line, ""))
			add_history(line);
		line = epur_str(line);
		if (!ft_strcmp(line, ""))
			continue ;
		cmd = init_cmds(ft_split(line, ' '));
		free(line);
		parse_cmd(cmd);
		if (!ft_strcmp(cmd->args[0], "exit"))
		{
			ft_printf("exit\n");
			if (!cmd->args[1])
				break ;
			if (!check_exit_args(cmd->args[1]))
				err_code = 255;
			else if (cmd->args[2])
			{
				ft_printf("minishell: exit: too many arguments\n");
				continue ;
			}
			else if (cmd->args[1])
			{
				err_code = ft_atoi(cmd->args[1]);
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