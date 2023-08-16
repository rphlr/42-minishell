/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:32:41 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/16 15:06:33 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <termios.h>

// clean ctr-c dans le terminal
void	set_termios(void)
{
	static struct termios	term;

	tcgetattr(0, &term);
	term.c_lflag = term.c_lflag & ~ECHOCTL;
	tcsetattr(0, 0, &term);
}

void	ft_signal(void)
{
	struct sigaction	s;

	s.sa_handler = SIG_IGN;
	sigemptyset(&s.sa_mask);
	s.sa_flags = 0;
	sigaction(SIGQUIT, &s, NULL);
	s.sa_handler = sg_manage;
	sigaction(SIGINT, &s, NULL);
}

void	sg_manage(int num)
{
	(void)num;
	if (num == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	update_signal_handler(int num)
{
	(void)num;
	if (num == SIGINT)
	{
		write(1, "\n", 1);
		rl_redisplay();
	}
	else if (num == SIGQUIT)
	{
		write(1, "quitting minishell\n", 19);
		rl_redisplay();
	}
}

void	update_signal(void)
{
	struct sigaction	s;

	s.sa_flags = 0;
	s.sa_handler = update_signal_handler;
	sigemptyset(&s.sa_mask);
	sigaction(SIGQUIT, &s, NULL);
	sigaction(SIGINT, &s, NULL);
}
