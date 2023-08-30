/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 19:51:40 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/30 19:53:54 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_read_history(char *filename)
{
	int			history_fd;
	char		*line;
	t_history	*history_head;

	history_head = NULL;
	history_fd = open(filename, O_CREAT | O_RDWR, 0644);
	if (history_fd == -1)
		ft_printf("minishell: can't open history file\n");
	else
	{
		line = get_next_line(history_fd);
		while (line)
		{
			line = rm_newline(line);
			add_history(line);
			add_to_history_list(&history_head, line);
			line = get_next_line(history_fd);
		}
	}
	return (history_fd);
}

int	add_to_history(char *line, int history_fd)
{
	t_history	*last_entry;
	t_history	*head;

	head = NULL;
	if (line && ft_strcmp(line, ""))
	{
		last_entry = head;
		while (last_entry && last_entry->next)
			last_entry = last_entry->next;
		if (!last_entry || ft_strcmp(line, last_entry->line))
		{
			add_history(line);
			ft_putendl_fd(line, history_fd);
			add_to_history_list(&head, line);
		}
		if (line_is_wspaces(line))
		{
			free(line);
			return (1);
		}
	}
	return (0);
}

void	loop_prompt(t_global *global, int history_fd)
{
	char		*rdm_prompt_clr;
	char		*line;

	while (1)
	{
		rdm_prompt_clr = ft_strjoin(ft_strjoin(ft_strjoin(
						"\033[", ft_itoa(get_random() % 7 + 31)), "m"), PROMPT);
		line = readline(rdm_prompt_clr);
		if (!check_token(line))
			break ;
		if (add_to_history(line, history_fd) || !ft_strcmp(line, ""))
			continue ;
		global->line = init_line(line, global);
		if (!global->line)
		{
			global->exit_code = 258;
			manage_exit(&global->exit_code);
			continue ;
		}
		free(line);
		global = parse_cmd(global);
		if (!ft_strcmp(global->line->cmds->cmd, ""))
			continue ;
		run_cmd(global);
	}
}

int	lsh_loop(t_global *global)
{
	int			history_fd;

	history_fd = ft_read_history(".minishell_history");
	loop_prompt(global, history_fd);
	return (global->exit_code);
}
