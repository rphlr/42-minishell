/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:53:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/07/31 14:48:03 by rrouille         ###   ########.fr       */
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
	int nbr_quote;
	int nbr_dquote;

	i = -1;
	nbr_quote = 0;
	nbr_dquote = 0;
	while (type[++i] != END)
	{
		if (type[i] == PIPE)
		{
			if (type[i + 1] == PIPE || type[i + 1] == END)
				return (PIPE_ERROR);
		}
		else if (type[i] == INPUT)
		{
			if (type[i + 1] == END)
				return (INPUT_ERROR);
		}
		else if (type[i] == OUTPUT)
		{
			if (type[i + 1] == END)
				return (OUTPUT_ERROR);
		}
		else if (type[i] == APPEND)
		{
			if (type[i + 1] == END)
				return (APPEND_ERROR);
		}
		else if (type[i] == HEREDOC)
		{
			if (type[i + 1] == END)
				return (HEREDOC_ERROR);
		}
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
		else if (type[i] == DOLLAR)
		{
			
		}
		else if (type[i] == TILDE)
		{
			
		}
		else if (type[i] == STAR)
		{
			
		}
		else if (type[i] == SEMICOLON)
		{
			if (type[i + 1] == SEMICOLON || type[i + 1] == END)
			{
				ft_printf("minishell: syntax error near unexpected token `;'\n");
				return (SEMICOLON_ERROR);
			}
		}
		else if (type[i] == COLON)
		{
			
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
	while (type[++i] != END || type[i] == PIPE)
	{
		if (type[i] != END && type[i] != PIPE)
		{
			if (i == 0 || type[i - 1] == PIPE)
				cmd[j++] = ft_strdup(tokens[i]);
			else
			{
				cmd[j - 1] = ft_strjoin(cmd[j - 1], " ");
				cmd[j - 1] = ft_strjoin(cmd[j - 1], tokens[i]);
			}
		}
	}
	if (j == 0)
		cmd = NULL;
	return (cmd);
}

int	count_cmd(t_token *type)
{
	int	i;
	int	nbr_cmd;

	i = -1;
	nbr_cmd = 0;
	while (type[++i] != END)
	{
		if (type[i] == PIPE)
			nbr_cmd++;
	}
	return (nbr_cmd + 1);
}

int	count_pipe(t_token *type)
{
	int	i;
	int	nbr_pipe;

	i = -1;
	nbr_pipe = 0;
	while (type[++i] != END)
	{
		if (type[i] == PIPE)
			nbr_pipe++;
	}
	return (nbr_pipe);
}

int	count_redirection(t_token *type)
{
	int	i;
	int	nbr_redirection;

	i = -1;
	nbr_redirection = 0;
	while (type[++i] != END)
	{
		if (type[i] == INPUT || type[i] == OUTPUT || type[i] == APPEND || type[i] == HEREDOC)
			nbr_redirection++;
	}
	return (nbr_redirection);
}

t_state	check_errors(t_token *type, char **tokens)
{
	t_state	state = VALID;

	state = validity_maker(type);
	if (state == QUOTE_ERROR)
		ft_printf("minishell: quote error\n");
	else if (state == DQUOTE_ERROR)
		ft_printf("minishell: dquote error\n");
	else if (state == PIPE_ERROR)
		ft_printf("minishell: syntax error near unexpected token `|'\n");
	else if (state == INPUT_ERROR)
	{
		if (!ft_strcmp(tokens[0], "<"))
			ft_printf("minishell: syntax error near unexpected token `newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `<'\n");
	}
	else if (state == OUTPUT_ERROR)
	{
		if (!ft_strcmp(tokens[0], ">"))
			ft_printf("minishell: syntax error near unexpected token `newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `>'\n");
	}
	else if (state == APPEND_ERROR)
	{
		if (!ft_strcmp(tokens[0], ">>"))
			ft_printf("minishell: syntax error near unexpected token `newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `>>'\n");
	}
	else if (state == HEREDOC_ERROR)
	{
		if (!ft_strcmp(tokens[0], "<<"))
			ft_printf("minishell: syntax error near unexpected token `newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `<<'\n");
	}
	else if (state == AND_ERROR)
	{
		if (!ft_strcmp(tokens[0], "&"))
			ft_printf("minishell: syntax error near unexpected token `&'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `&&'\n");
	}
	else if (state == OR_ERROR)
		ft_printf("minishell: syntax error near unexpected token `||'\n");
	if (state != VALID)
		return (state);
	return (VALID);
}

void	print_infos(t_cmd *cmd)
{
	int	i;

	ft_printf(C_RED""C_UNDERLINE""C_BOLD"TOKENS\n"C_RESET);
	ft_printf(C_RED"Nombre de tokens\t= "C_RESET""C_GREEN"%d\n"C_RESET, cmd->nbr_token);
	ft_printf(C_RED"tokens reçus et traités\t= "C_RESET);
	i = -1;
	while (cmd->token[++i])
	{
		ft_printf(C_GREEN);
		if (cmd->type[i + 1] == END)
			ft_printf("%s", cmd->token[i]);
		else
			ft_printf("%s, ", cmd->token[i]);
	}
	ft_printf("\n"C_RESET);
	ft_printf(C_RED"Type de tokens\t\t= "C_RESET);
	i = -1;
	while (cmd->type[++i] != END)
	{
		ft_printf(C_GREEN);
		if (cmd->type[i] == WORD)
			ft_printf("WORD");
		else if (cmd->type[i] == DOLLAR)
			ft_printf("DOLLAR");
		else if (cmd->type[i] == TILDE)
			ft_printf("TILDE");
		else if (cmd->type[i] == STAR)
			ft_printf("STAR");
		else if (cmd->type[i] == PIPE)
			ft_printf("PIPE");
		else if (cmd->type[i] == INPUT)
			ft_printf("INPUT");
		else if (cmd->type[i] == OUTPUT)
			ft_printf("OUTPUT");
		else if (cmd->type[i] == HEREDOC)
			ft_printf("HEREDOC");
		else if (cmd->type[i] == APPEND)
			ft_printf("APPEND");
		else if (cmd->type[i] == SEMICOLON)
			ft_printf("SEMICOLON");
		else if (cmd->type[i] == COLON)
			ft_printf("COLON");
		else if (cmd->type[i] == AND)
			ft_printf("AND");
		else if (cmd->type[i] == OR)
			ft_printf("OR");
		else if (cmd->type[i] == CLOSED_QUOTE)
			ft_printf("CLOSED_QUOTE");
		else if (cmd->type[i] == NOT_CLOSED_QUOTE)
			ft_printf("NOT_CLOSED_QUOTE");
		else if (cmd->type[i] == CLOSED_DQUOTE)
			ft_printf("CLOSED_DQUOTE");
		else if (cmd->type[i] == NOT_CLOSED_DQUOTE)
			ft_printf("NOT_CLOSED_DQUOTE");
		else if (cmd->type[i] == OPTIONS)
			ft_printf("OPTION");
		else
			ft_printf("UNKNOWN");
		if (cmd->type[i + 1] != END)
			ft_printf(", ");
	}
	ft_printf("\n"C_RESET);

	ft_printf(C_RED""C_UNDERLINE""C_BOLD"\nCOMMANDES\n"C_RESET);
	ft_printf(C_RED"Nombre de commandes\t= "C_RESET""C_GREEN"%d\n", cmd->nbr_cmd);
	i = -1;
	while (cmd->cmd[++i])
	{
		ft_printf(C_RED"Commande "C_RESET""C_GREEN"%d"C_RESET""C_RED"\t\t= "C_RESET, i + 1);
		ft_printf(C_GREEN"%s\n"C_RESET, cmd->cmd[i]);
	}
	
	ft_printf(C_RED""C_UNDERLINE""C_BOLD"\nREDIRECTIONS\n"C_RESET);
	ft_printf(C_RED"Nombre de pipes\t\t= "C_RESET""C_GREEN"%d\n"C_RESET, cmd->nbr_pipe);
	ft_printf(C_RED"Nombre de redirections\t= "C_RESET""C_GREEN"%d\n"C_RESET, cmd->nbr_redirection);
}

t_cmd	*init_cmds(char **tokens)
{
	t_cmd	*cmd;
	// t_global	*global;
	t_state	state = VALID;

	cmd = ft_gc_malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->token = tokens;
	cmd->type = init_tokens_type(tokens);
	state = check_errors(cmd->type, tokens);
	if (state)
		return (NULL);
	cmd->cmd = config_cmds(tokens, cmd->type);
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->pipe = NULL;
	cmd->heredoc = NULL;
	cmd->nbr_cmd = count_cmd(cmd->type);
	cmd->nbr_token = ft_tablen(tokens);
	cmd->nbr_pipe = count_pipe(cmd->type);
	cmd->nbr_redirection = count_redirection(cmd->type);
	// ft_printf("nbr_cmd = %d\n", cmd->nbr_cmd);
	// ft_printf("nbr_token = %d\n", cmd->nbr_token);
	// ft_printf("nbr_pipe = %d\n", cmd->nbr_pipe);
	// ft_printf("nbr_redirection = %d\n", cmd->nbr_redirection);
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

	cmd = global->cmd;
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
		if (!cmd)
		{
			global->exit_code = 258;
			continue ;
		}
		print_infos(cmd); // delete this line when done
		free(line);
		parse_cmd(global, cmd);
		if (!ft_strcmp(cmd->token[0], "exit"))
		{
			global->exit_code = 0;
			ft_printf("exit\n");
			if (!cmd->token[1])
				break ;
			if (!check_exit_token(cmd->token[1]))
				global->exit_code = 255;
			else if (cmd->token[2])
			{
				ft_printf("minishell: exit: too many arguments\n");
				continue ;
			}
			else if (cmd->token[1])
			{
				global->exit_code = ft_atoi(cmd->token[1]);
			 	if (global->exit_code < 0 || global->exit_code > 255)
					global->exit_code %= 256;
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
	return (global->exit_code);
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
	return (err_code);
}