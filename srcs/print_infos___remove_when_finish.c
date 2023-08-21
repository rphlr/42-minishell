/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_infos___remove_when_finish.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:44:36 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/18 23:48:06 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_infos(t_line *line)
{
	int	i;

	ft_printf(C_RED""C_UNDERLINE""C_BOLD"TOKENS\n"C_RESET);
	ft_printf(C_RED"Nombre de tokens\t= "C_RESET""C_GREEN"%d\n"C_RESET,\
		line->count->nbr_tokens);
	ft_printf(C_RED"tokens reçus et traités\t= "C_RESET);
	i = -1;
	while (line->token[++i])
	{
		ft_printf(C_GREEN);
		if (line->type[i + 1] == END)
			ft_printf("%s", line->token[i]);
		else
			ft_printf("%s, ", line->token[i]);
	}
	ft_printf("\n"C_RESET);
	ft_printf(C_RED"Type de tokens\t\t= "C_RESET);
	i = -1;
	while (line->type[++i] != END)
	{
		ft_printf(C_GREEN);
		if (line->type[i] == WORD)
			ft_printf("WORD");
		else if (line->type[i] == DOLLAR)
			ft_printf("DOLLAR");
		else if (line->type[i] == TILDE)
			ft_printf("TILDE");
		else if (line->type[i] == STAR)
			ft_printf("STAR");
		else if (line->type[i] == PIPE)
			ft_printf("PIPE");
		else if (line->type[i] == INPUT)
			ft_printf("INPUT");
		else if (line->type[i] == OUTPUT)
			ft_printf("OUTPUT");
		else if (line->type[i] == HEREDOC)
			ft_printf("HEREDOC");
		else if (line->type[i] == APPEND)
			ft_printf("APPEND");
		else if (line->type[i] == SEMICOLON)
			ft_printf("SEMICOLON");
		else if (line->type[i] == COLON)
			ft_printf("COLON");
		else if (line->type[i] == AND)
			ft_printf("AND");
		else if (line->type[i] == OR)
			ft_printf("OR");
		else if (line->type[i] == CLOSED_QUOTE)
			ft_printf("CLOSED_QUOTE");
		else if (line->type[i] == NOT_CLOSED_QUOTE)
			ft_printf("NOT_CLOSED_QUOTE");
		else if (line->type[i] == CLOSED_DQUOTE)
			ft_printf("CLOSED_DQUOTE");
		else if (line->type[i] == NOT_CLOSED_DQUOTE)
			ft_printf("NOT_CLOSED_DQUOTE");
		else if (line->type[i] == OPTIONS)
			ft_printf("OPTION");
		else
			ft_printf("UNKNOWN");
		if (line->type[i + 1] != END)
			ft_printf(", ");
	}
	ft_printf("\n"C_RESET);
	ft_printf(C_RED""C_UNDERLINE""C_BOLD"\nCOMMANDES\n"C_RESET);
	ft_printf(C_RED"Nombre de commandes\t= "C_RESET""C_GREEN"%d\n", line->count->nbr_cmds);
	i = -1;
	while (line->cmds->cmd[++i])
	{
		ft_printf(C_RED"Commande "C_RESET""C_GREEN"%d"C_RESET""C_RED"\t\t= "C_RESET, i + 1);
		ft_printf(C_GREEN"%s\n"C_RESET, line->cmds->cmd[i]);
	}
	ft_printf(C_RED""C_UNDERLINE""C_BOLD"\nREDIRECTIONS\n"C_RESET);
	ft_printf(C_RED"Nombre de pipes\t\t= "C_RESET""C_GREEN"%d\n"C_RESET, line->count->nbr_pipes);
	ft_printf(C_RED"Nombre de redirections\t= "C_RESET""C_GREEN"%d\n"C_RESET, line->count->nbr_inputs + line->count->nbr_outputs + line->count->nbr_heredocs + line->count->nbr_appends);
}
