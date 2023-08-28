/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:28:03 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/28 12:03:18 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ---------<<INCLUDES>>--------- */
// Librairies
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
# include <termios.h>
# include <dirent.h>

/* ---------<<MACROS>>--------- */
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

// Create a random colored prompt
# define PROMPT "\033[1m\033[7m\033[4m\033[41m Les pros du HTML >\033[0m "

// Define errors print
# define ERR_QUOTE "minishell: quote error\n"
# define ERR_DQUOTE "minishell: dquote error\n"
# define ERR_PARSE "minishell: syntax error near unexpected token `%s'\n"
# define ERR_OPTION "option error `%s'\n"
# define ERR_CD "minishell: cd: %s: %s\n"
# define ERR_CD_HOME "minishell: cd: HOME not set\n"

/* ---------<<STRUCTURES>>--------- */
// Types of tokens
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
	IGNORE,
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
}	t_token;

// State of the parsing
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

// Types of redirections
typedef enum s_redirectiontype
{
	NO_REDIRECTION,
	INPUT_REDIRECTION,
	OUTPUT_REDIRECTION,
	APPEND_REDIRECTION,
	HEREDOC_REDIRECTION
}						t_redirectiontype;

// Options for echo
typedef enum s_option
{
	NO_OPTION,
	OPTION_N,
	INVALID_OPTION
}						t_option;

// History
typedef struct s_history
{
	char				*line;
	struct s_history	*next;
}	t_history;

// Redirections and heredocs
typedef struct s_redirection
{
	t_redirectiontype	type;
	char				*filename;
	char				*limiter;
	int					type_hd;
}						t_redirection;

// Environment variables
typedef struct s_env
{
	char				*name;
	char				*value;
	struct s_env		*next;
}						t_env;

// Commands
typedef struct s_cmds
{
	char				*cmd;
	t_redirection		*redir;
	struct s_cmds		*next;
}						t_cmds;

// Count tokens
typedef struct s_count
{
	int					nbr_tokens;
	int					nbr_cmds;
	int					nbr_pipes;
	int					nbr_inputs;
	int					nbr_outputs;
	int					nbr_appends;
	int					nbr_heredocs;
	int					nbr_colons;
	int					nbr_semicolons;
	int					nbr_ands;
	int					nbr_ors;
	int					nbr_options;
	bool				special_cases;
}						t_count;

// Line
typedef struct s_line
{
	char				**token;
	t_token				*type;
	t_cmds				*cmds;
	bool				pipe;
	bool				heredoc;
	t_count				*count;
}						t_line;

// Global structure
typedef struct s_global
{
	int					exit_code;
	char				*home_folder;
	t_env				*env;
	t_line				*line;
}						t_global;

/* ---------<<PROTOTYPES>>--------- */
// Builtins
void					ft_echo(char *cmd, t_global *global);
char					*get_env_value(char *name, t_env *env);
void					ft_env(t_global *global);
void					ft_pwd(t_line *line);
void					ft_export(t_global *global, t_line *line);
void					ft_cd(char *cmd, t_global *global);
void					ft_unset(t_global *global, t_line *line);
void					ft_exit(t_global *global);

// Parsing
t_state					check_token_errors(t_token *type, char **tokens,
							t_state *error_table);
int						check_options_syntax(char *token);
bool					check_token(char *line);
t_state					check_errors(t_token *type, char **tokens,
							t_global *global);
char					*epur_str(char *line);
t_global				*parse_cmd(t_global *global);
t_count					*count_types(t_token *type);
t_state					ft_error(t_token *type, char **tokens,
							t_global *global);
t_cmds					*init_cmds(char **tokens, t_token *type);
t_line					*init_line(char *line, t_global *global);
t_global				*init_global(char **envp);
char					*format_options(char *token);
char					**parsed_line(char *line);
t_token					*init_tokens_type(char **tokens);
char					*ft_remove_char(char *str, char c);

// Execution
pid_t					manage_pid(pid_t *new_pid);
int						manage_exit(int *new_code);
void					run_cmd(t_global *global);

// Signals
void					ft_signal(void);
void					sigint_manage(int num);
void					set_termios(void);

#endif
