/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_easter_egg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 10:35:48 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/30 16:39:18 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_easter_egg(void)
{
	ft_printf(C_YELLOW"\n");
	ft_printf("	███    ███ ██ ███    ██ ██ ███████ ██   ██ ███████ ██\t██\n");
	ft_printf("	████  ████ ██ ████   ██ ██ ██      ██   ██ ██      ██\t██\n");
	ft_printf("	██ ████ ██ ██ ██ ██  ██ ██ ███████ ███████ █████   ██\t██\n");
	ft_printf("	██  ██  ██ ██ ██  ██ ██ ██      ██ ██   ██ ██      ██\t██\n");
	ft_printf("	██      ██ ██ ██   ████ ██ ███████ ██   ██ ███████ ███████ ███████\n");
	ft_printf("\n"C_RESET);
	ft_printf("This is a shell, not a game.\n");
	ft_printf("You can't play with it.\n");
	ft_printf("But you can play with your shell.\n");
	ft_printf("So, play with your shell.\n");
	ft_printf("\n");
}
