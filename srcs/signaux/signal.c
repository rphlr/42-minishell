/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:32:41 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/26 16:50:05 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// #include <readline/readline.h>
// #include <readline/history.h>
// #include <termios.h>

//c_lflag = 1011 0001      1011 0001
//ECHOCTL = 0000 0001 ~ -> 1111 1110  &
//          0000 0001      1011 0000
// clean ctr-c dans le terminal

void handle_sigpipe(int signo)
{
	(void) signo;
}

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
	s.sa_handler = sigint_manage;
	sigaction(SIGINT, &s, NULL);
	signal(SIGPIPE, handle_sigpipe);
}

void	sigint_manage(int num)
{
	(void) num;
	int exit_code;
	pid_t reset_pid;
	pid_t child_pid;

	child_pid = manage_pid(NULL);
	reset_pid = -1;
	if (child_pid > 0)
	{
        ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
        kill(child_pid, SIGINT);
		exit_code = 130;
    } else
	{
        ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		exit_code = 1;
    }
	manage_exit(&exit_code);
    manage_pid(&reset_pid);
}

// void	update_signal_handler(int num)
// {
// 	(void)num;
// 	if (num == SIGINT)
// 	{
// 		ft_printf("C'est quoi ça\n");
// 		rl_redisplay();
// 	}
// 	if (num == SIGQUIT)
// 	{
// 		ft_printf("quitting minishell\n");
// 		rl_redisplay();
// 	}
// }

// void	update_signal(void)
// {
// 	struct sigaction	s;

// 	s.sa_flags = 0;
// 	s.sa_handler = update_signal_handler;
// 	sigemptyset(&s.sa_mask);
// 	sigaction(SIGQUIT, &s, NULL); //crt bcklash
// 	sigaction(SIGINT, &s, NULL); //crt C
// }
