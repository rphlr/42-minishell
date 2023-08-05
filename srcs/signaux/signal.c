:/* ************************************************************************** */
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

#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void  signal()
{
  struct sigaction  s;

  s.sa_handler = SIG_IGN;
  sigemptyset(&s.sa_mask);
  s.sa_flags = 0;
  sigaction(SIGQUIT, &s, NULL); //ctr-\

  s.sa_handler = sg_manage;// function crl-c
  sigaction(SIGINT, &s, NULL);
}

void  sg_manage(int num)
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

void  update_signal_handler(int num)
{
  (void)num;

  if(num == SIGINT)
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

void  update_signal(void)
{
  struct sigaction s;

  s.sa_flags = 0;
  s.sa_handler = update_signal_handler;
  sigemptyset(&s.sa_mask);
  sigaction(SIGQUIT, &s, NULL); //crt bcklash
  sigaction(SIGINT, &s, NULL); //crt C
}



// SIGINT is the interrupt signal and is raised when you press Ctrl+C.
// Its default behavior is to terminate the process. The SIGINT signal can be
// dis-positioned, which means one can change the default behavior (by calling
// sighandler, or setting it SIG_IGN).
//void	signal_use(void)
//{
//	signal(SIGQUIT, pass);
//	signal(SIGINT, prompt_calling);
//}

// the function prompt calling ignores the actual prompt that we put before
// doing ctr+C, it also offers a new empty line to put a new prompt in.
//void	promt_calling(int key)
//{
//	(void)key;
//	rl_replace_line("", 0);
//	rl_on_new_line();
//	ft_printf("\n");
//	rl_redisplay();
//}
