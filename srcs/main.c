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
// ❌: loop
// ❌: Have a working history
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
// ❌: exit
// BONUS: Implement && and ||
// BONUS: Implement Wilcard * (globbing)

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

t_env	*init_env(char **envp)
{
  t_env	*env;
  int i;

  i = -1;
  env = ft_gc_malloc(sizeof(t_env));
  if (!env)
    return (NULL);  while (envp[++i])
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


bool	check_args(char *line)
{
	if (!line || !ft_strcmp(line, "exit"))
	{
		free(line);
		ft_printf("❯ Exiting minishell...\n");
		return false;
	}
	return true;
}

void	execute(t_cmd *cmd)
{
	(void) cmd;
}

void	lsh_loop(void)
{
	char	*line;
	t_cmd	*cmd;

	cmd = NULL;
	while (1)
	{
		line = readline(PROMPT);
		if (!check_args(line))
			break ;
		if (line[0])
			add_history(line);
		cmd = init_cmds(ft_split(line, ' '));
		execute(cmd);
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
  t_env	*env;

  (void)ac;
  (void)av;
  env = init_env(envp);
  if (!env)
    return (1);
  lsh_loop();
  return (0);
}

void  shell_loop(t_shell *shell)
{
  while (TRUE)
  {
      signals(&shell->mirror_termios);

  }
}
