/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:32:20 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/19 00:15:27 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ✅: display prompt when minishell is ready to receive input
// ✅: read input
// ✅: parse input
// ❌: Search and launch the right executable (based on the PATH variable
//			or by using relative or absolute path)
// ❌: display output
// ✅: loop
// ✅: Have a working history
// ❌: Do not use more than one global variable, think about it and be ready
//			to explain why you do it.
// ❌: Do not interpret unclosed quotes or unspecified special characters
//			like \ (eg with $’\n’)
// ❌: Handle ' and " (quotes) correctly
// ❌: Handle redirections > >> < <<
// ❌: Handle pipes | correctly
// ❌: Handle environment variables ($ followed by characters)
// ❌: Handle $? (exit code of the previous program)
// ✅: Handle ctrl-C ctrl-D ctrl-\ correctly
// ✅: Implement echo with option ’-n’
// ✅: Implement cd with only a relative or absolute path
// ✅: Implement pwd without any options
// ✅: Implement export without any options
// ✅: Implement unset without any options
// ✅: Implement env without any options and any arguments
// ✅: Implement exit without any options
// ✅: exit
// BONUS
// ✅: Detect Wilcard * (globbing)
// ✅: Detect && and ||

static int	line_is_spaces(char *line)
{
	while (*line)
	{
		if (*line++ != ' ')
			return (0);
	}
	return (1);
}

static char	*rm_newline(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\n')
		{
			line[i] = '\0';
			break ;
		}
		i++;
	}
	return (line);
}

static int	lsh_loop(t_global *global)
{
	char	*line;
	char	**tokens;
	int		history_fd;

	history_fd = open(".minishell_history", O_CREAT | O_RDWR, 0644);
	if (history_fd == -1)
		ft_printf("minishell: can't open history file\n");
	else
	{
		line = get_next_line(history_fd);
		while (line)
		{
			line = rm_newline(line);
			add_history(line);
			line = get_next_line(history_fd);
		}
	}
	while (1)
	{
		line = readline(PROMPT);
		if (!check_token(line))
			break ;
		if (line_is_spaces(line))
			continue ;
		if (line && ft_strcmp(line, ""))
		{
			add_history(line);
			ft_putendl_fd(line, history_fd);
		}
		tokens = parsed_line(line);
		if (!ft_strcmp(line, ""))
			continue ;
		global->line = init_line(tokens);
		if (!global->line)
		{
			global->exit_code = 258;
			continue ;
		}
		free(line);
		parse_cmd(global, global->line);
		run_cmd(global);
	}
	return (global->exit_code);
}

int	main(int ac, char **av, char **envp)
{
	t_global	*global;
	int			err_code;

	(void)ac;
	(void)av;
    set_termios();
    ft_signal();
    global = init_global(envp);
	if (!global)
		return (1);
	err_code = lsh_loop(global);
	exit(err_code);
}
