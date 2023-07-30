/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 07:34:38 by rrouille          #+#    #+#             */
/*   Updated: 2023/07/26 09:15:07 by rrouille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

char *ignore_quotes(const char *str)
{
    int i = 0;
    int j = 0;
    char *result = (char *)ft_gc_malloc((ft_strlen(str) + 1) * sizeof(char)); // Allocate memory for the result

    if (!result)
    {
        // Handle memory allocation failure
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    while (str[i])
    {
        if (str[i] != '\'' && str[i] != '\"') // Copy characters except quotes
            result[j++] = str[i];
        i++;
    }
    result[j] = '\0'; // Null-terminate the result string
    return result;
}

// char	*ignore_quotes(char *str)
// {
// 	int	i;
// 	int	j;

// 	i = -1;
// 	j = 0;
// 	while (str[++i])
// 	{
// 		if (str[i] == '\'' || str[i] == '\"')
// 			continue ;
// 		str[j++] = str[i];
// 	}
// 	str[j] = '\0';
// 	return (str);
// }

int	check_syntax(char *str)
{
	int	i;
	int	nbr_of_simple_cote;
	int	nbr_of_double_cote;

	i = -1;
	nbr_of_simple_cote = 0;
	nbr_of_double_cote = 0;
	while (str[++i])
	{
		if (str[i] == '\'')
			nbr_of_simple_cote++;
		if (str[i] == '\"')
			nbr_of_double_cote++;
	}
	if (nbr_of_simple_cote % 2 != 0 || nbr_of_double_cote % 2 != 0)
		return (0);
	return (1);
}

int	check_echo_token(char **str)
{
	int	i;
	int	j;
	int	arg;

	j = 0;
	i = 0;
	arg = 0;
	while (str[++i])
	{
		if (str[i][j++] == '-' && str[i][j] == 'n')
		{
			while (str[i][j] == 'n')
				j++;
			arg = 1;
		}
		if (str[i][j++] == '-' && str[i][j] == 'e')
		{
			while (str[i][j] == 'e')
				j++;
			arg = 2;
		}
	}
	return (arg);
}

void	echo_print_special_char(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{

		str = ignore_quotes(str);
		if (str[i] == '\\')
		{
			if (str[i + 1] == 'n')
				ft_printf("\n");
			else if (str[i + 1] == 't')
				ft_printf("\t");
			else if (str[i + 1] == 'v')
				ft_printf("\v");
			else if (str[i + 1] == 'b')
				ft_printf("\b");
			else if (str[i + 1] == 'r')
				ft_printf("\r");
			else if (str[i + 1] == 'f')
				ft_printf("\f");
			else if (str[i + 1] == 'a')
				ft_printf("\a");
			else if (str[i + 1] == '\\')
			{
				if (str[i + 2] == 'n')
					ft_printf("\n");
				else if (str[i + 2] == 't')
					ft_printf("\t");
				else if (str[i + 2] == 'v')
					ft_printf("\v");
				else if (str[i + 2] == 'b')
					ft_printf("\b");
				else if (str[i + 2] == 'r')
					ft_printf("\r");
				else if (str[i + 2] == 'f')
					ft_printf("\f");
				else if (str[i + 2] == 'a')
					ft_printf("\a");
				else if (str[i + 2] == '\\')
					ft_printf("\\");;
				i++;
			}
			else if (str[i + 1] == '0')
				ft_printf("\0");
			else
				ft_printf("%c", str[i + 1]);
			i++;
		}
		else
			ft_printf("%c", str[i]);
	}
}

void ft_echo(t_cmd *cmd)
{
    int arg = check_echo_token(cmd->token);

    printf("arg = %d\n", arg);

    if (arg == 1) // Check if -n is present
    {
        // Skip the newline at the end
        for (int i = 1; cmd->token[i]; i++)
        {
            cmd->token[i] = ignore_quotes(cmd->token[i]);
            printf("%s", cmd->token[i]);
            if (cmd->token[i + 1])
                printf(" ");
        }
    }
    else if (arg == 2) // Check if -e is present
    {
        // Process arguments with special characters
        for (int i = 1; cmd->token[i]; i++)
        {
            echo_print_special_char(ignore_quotes(cmd->token[i]));
            if (cmd->token[i + 1])
                printf(" ");
        }
    }
    else // No special options (-n or -e)
    {
        // Process arguments normally
        for (int i = 1; cmd->token[i]; i++)
        {
            cmd->token[i] = ignore_quotes(cmd->token[i]);
            printf("%s", cmd->token[i]);
            if (cmd->token[i + 1])
                printf(" ");
        }
        printf("\n"); // Print newline at the end
    }
}


// void	ft_echo(t_cmd *cmd)
// {
// 	int	i;
// 	int	arg;
// 	int	j;

// 	i = 0;
// 	arg = 0;
// 	j = 0;
// 	while (cmd->token[++i])
// 	{
// 		arg = check_echo_token(cmd->token);
// 		printf("arg = %d\n", arg);
// 		if (!ft_strstart(cmd->token[i], "-n") && !ft_strstart(cmd->token[i], "-e"))
// 		{
// 			printf("cmd->token[i] = %s\n", cmd->token[i]);
// 			if (j)
// 				ft_printf(" ");
// 			if (!check_syntax(cmd->token[i]))
// 				return ; // need to add syntax
// 			else if (arg == 2)
// 				echo_print_special_char(cmd->token[i]);
// 			else
// 			{
// 				cmd->token[i] = ignore_quotes(cmd->token[i]);
// 				ft_printf("%s", cmd->token[i]);
// 			}
// 			j++;
// 		}
// 	}
// 	if (arg != 1)
// 		ft_printf("\n");
// }