/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:28:03 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/15 18:22:08 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../mylib/includes/mylib.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

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

// # define PROMPT C_CYAN" rapidshell > "C_RESET
# define PROMPT "\033[1m\033[7m\033[4m\033[41m Les pros du HTML >\033[0m "

typedef enum s_token
{
	WORD,
	PIPE,
	INPUT,
	OUTPUT,
	APPEND,
	HEREDOC,
	AND,
	OR,
	SEMICOLON,
	CLOSED_QUOTE,
	CLOSED_DQUOTE,
	NOT_CLOSED_QUOTE,
	NOT_CLOSED_DQUOTE,
	OPTIONS,
	STAR,
	DOLLAR,
	TILDE,
	COLON,
	BS_NEWLINE,
	BS_TAB,
	BS_VERTICAL_TAB,
	BS_BACKSPACE,
	BS_CARRIAGE_RETURN,
	BS_FORM_FEED,
	BS_BELL,
	BS_ESCAPE,
	BS_BACKSLASH,
	END
}						t_token;

typedef enum state
{
	VALID,
	QUOTE_ERROR,
	DQUOTE_ERROR,
	PIPE_ERROR,
	INPUT_ERROR,
	OUTPUT_ERROR,
	APPEND_ERROR,
	HEREDOC_ERROR,
	AND_ERROR,
	OR_ERROR,
	SEMICOLON_ERROR,
	OPTIONS_ERROR
}						t_state;

typedef enum s_redirectiontype
{
	NO_REDIRECTION,
	INPUT_REDIRECTION,
	OUTPUT_REDIRECTION,
	APPEND_REDIRECTION
}						t_redirectiontype;

typedef enum s_option
{
	NO_OPTION,
	OPTION_N,
	INVALID_OPTION
}						t_option;

typedef struct s_redirection
{
	t_redirectiontype	type;
	char				*filename;
}						t_redirection;

typedef struct s_env
{
	char				*name;
	char				*value;
	bool				is_env;
	struct s_env		*next;
}						t_env;

typedef struct s_cmd
{
	char				**token;
	t_token				*type;
	char				**cmd;
	t_redirection		*input;
	t_redirection		*output;
	bool				pipe;
	bool				heredoc;
	int					heredoc_fd;
	int					nbr_cmd;
	int					nbr_token;
	int					nbr_pipe;
	int					nbr_redirection;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_global
{
	int					exit_code;
	int					pid;
	t_env				*env;
	t_cmd				*cmd;
}						t_global;

/* FUNCTIONS */
// *---* builtins *---*
void					ft_echo(t_global *global);
char					*get_env_value(char *name, t_global *global);
void					ft_env(t_global *global);
void					ft_pwd(t_cmd *cmd);
void					ft_export(t_global *global, t_cmd *cmd);
void					ft_cd(t_global *global);
void					ft_unset(t_global *global, t_cmd *cmd);
void					ft_exit(t_global *global);

// *---* parsing *---*
t_state					check_token_errors(t_token *type, char **tokens,
							t_state *error_table);
int						check_options_syntax(char *token);
bool					check_token(char *line);
t_state					check_errors(t_token *type, char **tokens);
char					*epur_str(char *line);
int						parse_cmd(t_global *global, t_cmd *cmd);
int						count_cmd(t_token *type);
int						count_pipe(t_token *type);
int						count_redirection(t_token *type);
t_state					ft_error(t_token *type, char **tokens);
t_cmd					*init_cmds(char **tokens);
t_global				*init_global(char **envp);
char					*format_options(char *token);
char					**parsed_line(char *line);
t_token					*init_tokens_type(char **tokens);

// *---* exec *---*
void					execute(t_global *global);

// *---* signals *---*
void					ft_signal(void);
void					sg_manage(int num);
void					update_signal_handler(int num);
void					update_signal(void);
void					set_termios(void);

// remove when finish
void					print_infos(t_cmd *cmd);

#endif