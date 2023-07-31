/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvillarr <mvillarr@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:32:41 by mvillarr          #+#    #+#             */
/*   Updated: 2023/07/30 13:31:44 by mvillarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell"i

// SIGINT is the interrupt signal and is raised when you press Ctrl+C.
// Its default behavior is to terminate the process. The SIGINT signal can be
// dis-positioned, which means one can change the default behavior (by calling
// sighandler, or setting it SIG_IGN).
void	signal_use(void)
{
	signal(SIGQUIT, pass);
	signal(SIGINT, prompt_calling);
}

// the function prompt calling ignores the actual prompt that we put before
// doing ctr+C, it also offers a new empty line to put a new prompt in.
void	promt_calling(int key)
{
	(void)key;
	rl_replace_line("", 0);
	rl_on_new_line();
	ft_printf("\n");
	rl_redisplay();	
}

// *---------------------------------*

void	signals(struct termios *mirror_termios)
{
        g_ctrl_c = FALSE;
        save_settings_and_remove_c(mirror_termios);
        signal_ctrl_backslash();
        signal_ctrl_c();
}

void	save_settings_and_remove_c(struct termios *mirror_termios)
{
        struct termios		termios_settings;

        tcgetattr(1, mirror_termios);
        tcgetattr(1, &termios_settings);
        termios_settings.c_lflag &= ~ECHOCTL;
        tcsetattr(1, TCSAFLUSH, &termios_settings);
}

void	signal_ctrl_c(void)
{
        struct sigaction	ctrl_c;

        ctrl_c.sa_handler = handle_sigint;
        ctrl_c.sa_flags = SA_RESTART;
        sigemptyset(&ctrl_c.sa_mask);
        sigaction(SIGINT, &ctrl_c, NULL);
}

void	signal_ctrl_backslash(void)
{
        struct sigaction	ctrl_back_slash;

        ctrl_back_slash.sa_handler = SIG_IGN;
        ctrl_back_slash.sa_flags = SA_RESTART;
        sigemptyset(&ctrl_back_slash.sa_mask);
        sigaction(SIGQUIT, &ctrl_back_slash, NULL);
}

// fct not used
void	handle_sigint(int sig_num)
{
        if (sig_num == SIGINT)
        {
          g_ctrl_c = TRUE;
          write(1, "\n", 2);
          rl_on_new_line();
          rl_replace_line("", 0);
          rl_redisplay();
        }
}

