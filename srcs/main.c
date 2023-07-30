/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:53:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/07/30 16:33:06 by rrouille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

// ✅: display prompt when minishell is ready to receive input
// ✅: read input
// ✅: parse input
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

t_state	validity_maker(t_token *type)
{
	int	i;
	int nbr_pipe;
	int nbr_redirection;
	int nbr_quote;
	int nbr_dquote;

	i = -1;
	nbr_pipe = 0;
	nbr_redirection = 0;
	nbr_quote = 0;
	nbr_dquote = 0;
	while (type[++i] != END)
	{
		if (type[i] == PIPE)
		{
			nbr_pipe++;
			if (type[i + 1] == PIPE || type[i + 1] == END)
				return (PIPE_ERROR);
		}
		else if (type[i] == INPUT)
		{
			nbr_redirection++;
			if (type[i + 1] == END)
				return (INPUT_ERROR);
		}
		else if (type[i] == OUTPUT)
		{
			nbr_redirection++;
			if (type[i + 1] == END)
				return (OUTPUT_ERROR);
		}
		else if (type[i] == APPEND)
		{
			nbr_redirection++;
			if (type[i + 1] == END)
				return (APPEND_ERROR);
		}
		else if (type[i] == HEREDOC)
		{
			nbr_redirection++;
			if (type[i + 1] == END)
				return (HEREDOC_ERROR);
		}
		// else if (type[i] == INPUT || type[i] == OUTPUT || type[i] == APPEND || type[i] == HEREDOC)
		// {
		// 	nbr_redirection++;
		// 	if (type[i + 1] == INPUT || type[i + 1] == OUTPUT
		// 		|| type[i + 1] == APPEND || type[i + 1] == PIPE)
		// 	{
		// 		ft_printf("minishell: syntax error near unexpected token `%s'\n",
		// 			type[i + 1] == INPUT ? "<" : type[i + 1] == OUTPUT ? ">" : type[i + 1] == APPEND ? ">>" : "<<");
		// 		return (OUTPUT_ERROR);
		// 	}
		// 	else if (type[i + 1] == END)
		// 		return (INPUT_ERROR);
		// 	ft_printf("nbr_redirection = %d\n", nbr_redirection);
		// }
		else if (type[i] == NOT_CLOSED_QUOTE)
			nbr_quote++;
		else if (type[i] == NOT_CLOSED_DQUOTE)
			nbr_dquote++;
		else if (type[i] == CLOSED_QUOTE)
			nbr_quote += 2;
		else if (type[i] == CLOSED_DQUOTE)
			nbr_dquote += 2;
		else if (type[i] == AND)
		{
			if (type[i + 1] == AND || type[i + 1] == END)
				return (AND_ERROR);
		}
		else if (type[i] == OR)
		{
			if (type[i + 1] == OR || type[i + 1] == END)
				return (OR_ERROR);
		}
		else if (type[i] == VARIABLES)
		{
			// if (type[i + 1] == VARIABLES || type[i + 1] == END)
			// {
			// 	ft_printf("minishell: syntax error near unexpected token `$'\n");
			// 	return (true);
			// }
		}
		else if (type[i] == WORD)
		{
			// if (type[i + 1] == WORD || type[i + 1] == END)
			// {
			// 	ft_printf("minishell: syntax error near unexpected token `newline'\n");
			// 	return (true);
			// }
		}
		// ft_printf("type[i] = %d\n", type[i]);
		// ft_printf("nbr_quote = %d\n", nbr_quote);
		// ft_printf("nbr_dquote = %d\n", nbr_dquote);
	}
	if (nbr_quote % 2 != 0)
	{
		ft_printf("quote error\n");
		return (QUOTE_ERROR);
	}
	if (nbr_dquote % 2 != 0)
	{
		ft_printf("dquote error\n");
		return (DQUOTE_ERROR);
	}
	ft_printf("nbr_pipe = %d\n", nbr_pipe);
	ft_printf("nbr_redirection = %d\n", nbr_redirection);
	return (VALID);
}

int ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	**config_cmds(char **tokens, t_token *type)
{
	int		i;
	int		j;
	char	**cmd;

	i = -1;
	j = 0;
	cmd = ft_gc_malloc(sizeof(char *) * (ft_tablen(tokens) + 1));
	while (type[++i] != END)
	{
		if (type[i] == WORD)
		{
			cmd[j++] = ft_strdup(tokens[i]);
			ft_printf("cmd[%d] = %s\n", j - 1, cmd[j - 1]);
		}
	}
	if (j == 0)
		cmd = NULL;
	return (cmd);
}

t_cmd	*init_cmds(char **tokens)
{
	t_cmd	*cmd;
	t_state	state = VALID;

	cmd = ft_gc_malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->token = tokens;
	cmd->type = init_tokens_type(tokens);
	state = validity_maker(cmd->type);
	if (state == QUOTE_ERROR)
		ft_printf("minishell: quote error\n");
	else if (state == DQUOTE_ERROR)
		ft_printf("minishell: dquote error\n");
	else if (state == PIPE_ERROR)
		ft_printf("minishell: syntax error near unexpected token `|'\n");
	else if (state == INPUT_ERROR)
		ft_printf("minishell: syntax error near unexpected token `<'\n");
	else if (state == OUTPUT_ERROR)
		ft_printf("minishell: syntax error near unexpected token `>'\n");
	else if (state == APPEND_ERROR)
		ft_printf("minishell: syntax error near unexpected token `>>'\n");
	else if (state == HEREDOC_ERROR)
		ft_printf("minishell: syntax error near unexpected token `<<'\n");
	else if (state == AND_ERROR)
		ft_printf("minishell: syntax error near unexpected token `&&'\n");
	else if (state == OR_ERROR)
		ft_printf("minishell: syntax error near unexpected token `||'\n");
	if (state != VALID)
		return (cmd);
	cmd->cmd = config_cmds(tokens, cmd->type);
	cmd->heredoc = NULL;
	// cmd->env = NULL;
	// cmd->global = NULL;
	cmd->nbr_cmd = 0;
	cmd->nbr_pipe = 0;
	cmd->nbr_redirection = 0;
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

int parse_cmd(t_global *global, t_cmd *cmd)
{
	int i;
	
	i = 0;
	while (cmd->token[i])
	{
		if (!ft_strcmp(cmd->token[i], "$HOME"))
		{
			cmd->token[i] = global->env->home;
		}
		else if (!ft_strcmp(cmd->token[i], "$PWD"))
		{
			cmd->token[i] = global->env->pwd;
		}
		else if (!ft_strcmp(cmd->token[i], "$OLDPWD"))
		{
			cmd->token[i] = global->env->oldpwd;
		}
		else if (!ft_strcmp(cmd->token[i], "$USER"))
		{
			cmd->token[i] = global->env->user;
		}
		else if (!ft_strcmp(cmd->token[i], "$SHELL"))
		{
			cmd->token[i] = global->env->shell;
		}
		else if (!ft_strcmp(cmd->token[i], "$?"))
		{
			cmd->token[i] = ft_itoa(global->exit_code);
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

int	lsh_loop(t_global *global)
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
		parse_cmd(global, cmd);
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
		else if (!ft_strcmp(cmd->token[0], "env"))
		{
			int	i = -1;
			ft_printf("USER=%s\n", global->env->user);
			ft_printf("PATH=");
			while (global->env->path[++i])
			{
				if (i == 0)
					ft_printf("%s", global->env->path[i]);
				else
					ft_printf(":%s", global->env->path[i]);
			}
			ft_printf("\n");
			ft_printf("HOME=%s\n", global->env->home);
			ft_printf("SHELL=%s\n", global->env->shell);
			ft_printf("PWD=%s\n", global->env->pwd);
			ft_printf("OLDPWD=%s\n", global->env->oldpwd);
		}
		run_cmd(cmd);
	}
	return (err_code);
}

t_global	*init_global(char **envp)
{
	t_global	*global;
	
	global = ft_gc_malloc(sizeof(t_global));
	global->exit_code = 0;
	global->env = init_env(envp);
	if (!global->env)
		return (NULL);
	global->cmd = NULL;
	return (global);
}

int	main(int ac, char **av, char **envp)
{
	// t_env		*env;
	t_global	*global;

	(void)ac;
	(void)av;
	global = init_global(envp);
	if (!global)
		return (1);
	int err_code = lsh_loop(global);
	exit (err_code);
}