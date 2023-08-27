/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 07:34:38 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/27 18:23:55 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_option(char *str)
{
	while (*++str)
	{
		if (*str != 'n')
			return (0);
	}
	return (1);
}

static void	echo_print(char *str)
{
	while (*str)
	{
		if (*str == '\\')
		{
			str++;
			ft_printf("%c", *str);
			str++;
			continue;
		}
		ft_printf("%c", *str++);
	}
}

//static char **split_cmd_to_tokens(char *cmd)
//{
//	int count = 0;
//	char **tokens;
//	char *token;
//	char *cmd_copy = ft_strdup(cmd);

//	token = ft_strtok(cmd_copy, " ");
//	while (token)
//	{
//		count++;
//		token = ft_strtok(NULL, " ");
//	}
//	cmd_copy = ft_strdup(cmd);
//	tokens = (char **)ft_gc_malloc((count + 1) * sizeof(char *));
//	count = 0;
//	token = ft_strtok(cmd_copy, " ");
//	while (token)
//	{
//		tokens[count] = ft_strdup(token);
//		count++;
//		token = ft_strtok(NULL, " ");
//	}
//	tokens[count] = NULL;
//	return tokens;
//}

// char **split_cmd_to_tokens(char *cmd) {
//     int count = 0;
//     char **tokens;
//     char *start = cmd;
//     char *token;
//     int in_quotes = 0;

//     // First pass: Count tokens
//     while (*start) {
//         while (*start == ' ') start++; // skip spaces

//         if (*start == '\'') {
//             in_quotes = !in_quotes; // toggle quote state
//             start++; // move to next char
//         }

//         if (!in_quotes) {
//             while (*start && *start != ' ') start++;
//         } else {
//             while (*start && *start != '\'') start++;
//             if (*start == '\'') {
//                 in_quotes = 0; // exit quote state
//                 start++; // move past the closing quote
//             }
//         }
//         count++;
//     }

//     tokens = (char **)malloc((count + 1) * sizeof(char *));
//     start = cmd;
//     count = 0;
//     in_quotes = 0;

//     // Second pass: Extract tokens
//     while (*start) {
//         while (*start == ' ') start++; // skip spaces

//         if (*start == '\'') {
//             in_quotes = !in_quotes;
//             token = start + 1; // start of token
//             start++; // move to next char
//         } else {
//             token = start; // start of token
//         }

//         if (!in_quotes) {
//             while (*start && *start != ' ') start++;
//         } else {
//             while (*start && *start != '\'') start++;
//             if (*start == '\'') {
//                 in_quotes = 0; // exit quote state
//                 start++; // move past the closing quote
//             }
//         }

//         tokens[count] = ft_strndup(token, start - token - (token[-1] == '\''));
//         count++;
//     }

//     tokens[count] = NULL;
//     return tokens;
// }

void	ft_echo(char *cmd)
{
	int	nwln;
	int i;
	char **tokens;

<<<<<<< HEAD
	nwln = 1;
=======
	nwln  = 1;
	tokens = split_cmd_to_tokens(cmd);
>>>>>>> 70258eb3ff850f618045b294be1b488fbfdff991
	i = 0;
	while (tokens[i] && ft_strcmp(tokens[i], "echo") != 0)
		i++;
	i++;
	while (tokens[i] && tokens[i][0] == '-' && check_option(tokens[i]))
	{
		nwln = 0;
		i++;
	}
	while (tokens[i])
	{
		echo_print(tokens[i]);
		i++;
		if (tokens[i])
			ft_printf(" ");
	}
	if (nwln)
		ft_printf("\n");
}
