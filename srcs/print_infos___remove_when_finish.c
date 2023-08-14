/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_infos___remove_when_finish.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:44:36 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/07 17:36:01 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_infos(t_cmd *cmd)
{
	int	i;

	ft_printf(C_RED""C_UNDERLINE""C_BOLD"TOKENS\n"C_RESET);
	ft_printf(C_RED"Nombre de tokens\t= "C_RESET""C_GREEN"%d\n"C_RESET,\
		cmd->nbr_token);
	ft_printf(C_RED"tokens reçus et traités\t= "C_RESET);
	i = -1;
	while (cmd->token[++i])
	{
		ft_printf(C_GREEN);
		if (cmd->type[i + 1] == END)
			ft_printf("%s", cmd->token[i]);
		else
			ft_printf("%s, ", cmd->token[i]);
	}
	ft_printf("\n"C_RESET);
	ft_printf(C_RED"Type de tokens\t\t= "C_RESET);
	i = -1;
	while (cmd->type[++i] != END)
	{
		ft_printf(C_GREEN);
		if (cmd->type[i] == WORD)
			ft_printf("WORD");
		else if (cmd->type[i] == DOLLAR)
			ft_printf("DOLLAR");
		else if (cmd->type[i] == TILDE)
			ft_printf("TILDE");
		else if (cmd->type[i] == STAR)
			ft_printf("STAR");
		else if (cmd->type[i] == PIPE)
			ft_printf("PIPE");
		else if (cmd->type[i] == INPUT)
			ft_printf("INPUT");
		else if (cmd->type[i] == OUTPUT)
			ft_printf("OUTPUT");
		else if (cmd->type[i] == HEREDOC)
			ft_printf("HEREDOC");
		else if (cmd->type[i] == APPEND)
			ft_printf("APPEND");
		else if (cmd->type[i] == SEMICOLON)
			ft_printf("SEMICOLON");
		else if (cmd->type[i] == COLON)
			ft_printf("COLON");
		else if (cmd->type[i] == AND)
			ft_printf("AND");
		else if (cmd->type[i] == OR)
			ft_printf("OR");
		else if (cmd->type[i] == CLOSED_QUOTE)
			ft_printf("CLOSED_QUOTE");
		else if (cmd->type[i] == NOT_CLOSED_QUOTE)
			ft_printf("NOT_CLOSED_QUOTE");
		else if (cmd->type[i] == CLOSED_DQUOTE)
			ft_printf("CLOSED_DQUOTE");
		else if (cmd->type[i] == NOT_CLOSED_DQUOTE)
			ft_printf("NOT_CLOSED_DQUOTE");
		else if (cmd->type[i] == OPTIONS)
			ft_printf("OPTION");
		else
			ft_printf("UNKNOWN");
		if (cmd->type[i + 1] != END)
			ft_printf(", ");
	}
	ft_printf("\n"C_RESET);
	ft_printf(C_RED""C_UNDERLINE""C_BOLD"\nCOMMANDES\n"C_RESET);
	ft_printf(C_RED"Nombre de commandes\t= "C_RESET""C_GREEN"%d\n", cmd->nbr_cmd);
	i = -1;
	while (cmd->cmd[++i])
	{
		ft_printf(C_RED"Commande "C_RESET""C_GREEN"%d"C_RESET""C_RED"\t\t= "C_RESET, i + 1);
		ft_printf(C_GREEN"%s\n"C_RESET, cmd->cmd[i]);
	}
	ft_printf(C_RED""C_UNDERLINE""C_BOLD"\nREDIRECTIONS\n"C_RESET);
	ft_printf(C_RED"Nombre de pipes\t\t= "C_RESET""C_GREEN"%d\n"C_RESET, cmd->nbr_pipe);
	ft_printf(C_RED"Nombre de redirections\t= "C_RESET""C_GREEN"%d\n"C_RESET, cmd->nbr_redirection);
}
