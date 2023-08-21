/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pips.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 17:29:45 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/21 19:07:11 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
allouer de la memoire pour que les pipes se connectent avec tous les processus
*/

static int **allocate_pipe(int nbr_of_child_process)
{
    
}

int **create_pipe(int nbr_of_child_processes)
{
    
}

void close_pipes_in_child_process(int **pipes, int nbr_of_child_processes, int i)
{

}

void    close_pipes_in_main_process(int **pipes, int nbr_of_child_processes)
{
    
}
