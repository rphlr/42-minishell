/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:32:41 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/16 14:11:52 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>

//c_lflag = 1011 0001      1011 0001
//ECHOCTL = 0000 0001 ~ -> 1111 1110  &
//          0000 0001      1011 0000
// clean ctr-c dans le terminal
/* struct sigaction s is a structure that specifies the action to be associated
 with specific signals. It's used to define a signal handler with the
 sigaction system call. */
void	set_termios(void)
{
	static struct termios	term;

	tcgetattr(0, &term);
	term.c_lflag = term.c_lflag & ~ECHOCTL;
	tcsetattr(0, 0, &term);
}

/*  SIGQUIT is typically sent to a process to tell it to terminate and dump core
 (ctrl+d)

 SIGINT is the signal sent to a process by its controlling terminal when a user
 wishes to interrupt the process (ctrl+c) */
void	ft_signal(void)
{
	struct sigaction	s;

	s.sa_handler = SIG_IGN;
	sigemptyset(&s.sa_mask);
	s.sa_flags = 0;
	sigaction(SIGQUIT, &s, NULL); //ctr-bck slash
	s.sa_handler = sigint_manage;// function crl-c
	sigaction(SIGINT, &s, NULL);
}

/* This function is the signal handler for SIGINT. When SIGINT is received, it
 writes a newline character to the terminal, replaces the current line with an
 empty string, and then redisplay the prompt. */
void	sigint_manage(int num)
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

/* It handles both SIGINT and SIGQUIT. For SIGINT, it behaves similarly to
 sigint_manage, but for SIGQUIT, it writes "quitting minishell\n"
 to the terminal.*/
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
		write (1, "quitting minishell\n", 19);
		rl_redisplay();
	}
}

/* This function is similar to ft_signal, but it sets up the signal handling
 using update_signal_handler instead of sg_manage. It's also handling both
 SIGQUIT and SIGINT. */
void	update_signal(void)
{
	struct sigaction	s;

	s.sa_flags = 0;
	s.sa_handler = update_signal_handler;
	sigemptyset(&s.sa_mask);
	sigaction(SIGQUIT, &s, NULL); //crt bcklash
	sigaction(SIGINT, &s, NULL); //crt C
}
