/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:28:03 by rrouille          #+#    #+#             */
/*   Updated: 2023/07/26 09:13:47 by rrouille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include "../mylib/includes/mylib.h"

// Colors definition
# define C_RED "\033[0;31m"
# define C_GREEN "\033[0;32m"
# define C_YELLOW "\033[0;33m"
# define C_BLUE "\033[0;34m"
# define C_MAGENTA "\033[0;35m"
# define C_CYAN "\033[0;36m"
# define C_BLACK "\033[30m"
# define C_WHITE "\033[37m"
# define C_RESET "\033[0m"

// Text effects definition
# define C_BOLD "\033[1m"
# define C_UNDERLINE "\033[4m"
# define C_BLINK "\033[5m"
# define C_REVERSE "\033[7m"
# define C_INVISIBLE "\033[8m"
# define C_STRIKETHROUGH "\033[9m"

// Background colors definition
# define C_BBLACK "\033[40m"
# define C_BWHITE "\033[47m"
# define C_BRED "\033[41m"
# define C_BGREEN "\033[42m"
# define C_BYELLOW "\033[43m"
# define C_BBLUE "\033[44m"
# define C_BMAGENTA "\033[45m"
# define C_BCYAN "\033[46m"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define PROMPT C_UNDERLINE""C_BOLD""C_BMAGENTA" Les pros du HTML >"C_RESET" "

typedef enum s_token
{
	WORD,
	PIPE,
	INPUT,
	OUTPUT,
	APPEND,
	SEMICOLON,
	VARIABLES,
	END
}	t_token;

typedef enum s_redirectiontype {
    NO_REDIRECTION,
    INPUT_REDIRECTION,
    OUTPUT_REDIRECTION,
    APPEND_REDIRECTION
} t_redirectiontype;

typedef struct s_redirection
{
	t_redirectiontype	type;
	char			*filename;
} t_redirection;

typedef struct s_env
{
	char	**path;
	char	*home;
	char	*pwd;
	char	*oldpwd;
	char	*user;
	char	*shell;
}	t_env;

typedef struct s_cmd t_cmd;

typedef struct s_global
{
	int		exit_code;
	t_env	*env;
	t_cmd	*cmd;
}	t_global;

typedef struct s_cmd
{
	char			**token;
	t_token			*type;
	char			*cmd;
	t_redirection	*input;
	t_redirection	*output;
	bool			pipe;
	t_env			*env;
	t_global		*global;
	struct s_cmd	*next;
}	t_cmd;


/* FUNCTIONS */
int	ft_strstart(char *str, char *start);
void	ft_echo(t_cmd *cmd);
t_token	*init_tokens_type(char **tokens);

#endif