/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:28:03 by rrouille          #+#    #+#             */
/*   Updated: 2023/07/16 18:27:11 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/signal.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <termios.h>
# include "../mylib/includes/mylib.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define PROMPT "minishell> "

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
	char			**args;
	char			*cmd;
	char			*redir_in;
	char			*redir_out;
	char			*redir_append;
	char			*pipe;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_global
{
	int		exit_code;
	t_env	*env;
	t_cmd	*cmd;
}	t_global;

// *---* signals *---*
void	parent_signal(void);
void	sigint_parent(int signum);
void	ctr_c_signal_parent(void);
void    child_signal(struct termios *mirror_termios);
void    sigint_child(void);
void    ctr_bl_signal(void);


#endif
