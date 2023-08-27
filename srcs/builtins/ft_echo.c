/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 07:34:38 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/27 14:20:09 by rrouille         ###   ########.fr       */
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

// static char **split_cmd_to_tokens(char *cmd)
// {
// 	int count = 0;
// 	char **tokens;
// 	char *token;
// 	char *cmd_copy = ft_strdup(cmd);

// 	token = ft_strtok(cmd_copy, " ");
// 	while (token)
// 	{
// 		count++;
// 		token = ft_strtok(NULL, " ");
// 	}
// 	cmd_copy = ft_strdup(cmd);
// 	tokens = (char **)ft_gc_malloc((count + 1) * sizeof(char *));
// 	if (!tokens)
// 		return NULL;
// 	count = 0;
// 	token = ft_strtok(cmd_copy, " ");
// 	while (token)
// 	{
// 		tokens[count++] = ft_strdup(token);
// 		// count++;
// 		token = ft_strtok(NULL, " ");
// 	}
// 	tokens[count + 1] = NULL;
// 	return tokens;
// }

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

// static char **split_cmd_to_tokens(char *cmd)
// {
//     int count = 0;
//     char **tokens;
//     char *start = cmd;
//     char *end;
//     int in_quotes = 0;

//     // Première passe: Compter les tokens
//     while (*start) {
//         while (*start == ' ') start++; // ignorer les espaces

//         end = start;
//         if (*start == '"' || *start == '\'') {
//             char quote_char = *start;
//             in_quotes = 1;
//             end++;
//             while (*end && *end != quote_char) end++;
//             if (*end == quote_char) {
//                 end++;
//             }
//         } else {
//             while (*end && *end != ' ') end++;
//         }
//         count++;
//         start = end;
//     }

//     tokens = (char **)malloc((count + 1) * sizeof(char *));
//     if (!tokens) return NULL;

//     start = cmd;
//     count = 0;

//     // Deuxième passe: Extraire les tokens
//     while (*start) {
//         while (*start == ' ') start++; // ignorer les espaces

//         end = start;
//         if (*start == '"' || *start == '\'') {
//             char quote_char = *start;
//             end++;
//             while (*end && *end != quote_char) end++;
//             if (*end == quote_char) {
//                 end++;
//             }
//             tokens[count] = ft_strndup(start + 1, end - start - 2); // +1 et -2 pour ignorer les guillemets
//         } else {
//             while (*end && *end != ' ') end++;
//             tokens[count] = ft_strndup(start, end - start);
//         }
//         count++;
//         start = end;
//     }

//     tokens[count] = NULL;
//     return tokens;
// }



void	ft_echo(char *cmd, t_global *global)
{
	int	nwln;
	int i;

	(void)cmd;
	nwln  = 1;
	i = 0;
	while (global->line->token[i] && ft_strcmp(global->line->token[i], "echo") != 0)
		i++;
	i++;
	while (global->line->token[i] && global->line->token[i][0] == '-' && check_option(global->line->token[i]))
	{
		nwln = 0;
		i++;
	}
	while (global->line->token[i])
	{
		echo_print(global->line->token[i]);
		i++;
		if (global->line->token[i])
			ft_printf(" ");
	}
	if (nwln)
		ft_printf("\n");
}

