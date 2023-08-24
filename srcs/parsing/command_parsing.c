/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:42:55 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/24 15:01:01 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_cmd(t_line *line)
{
	DIR				*dir;
	struct dirent	*entry;
	int				j;
	int				i;
	// int				new_count;
	char			**token_ptr;
	t_token			*type_ptr;
	int				original_count;
	char			**entries_array;
	int				entries_count;
	char			**old_tokens;
	t_token			*old_types;

	// new_count = 0;
	original_count = 0;
	while (line->token[original_count] != NULL)
		original_count++;
	token_ptr = line->token;
	type_ptr = line->type;
	while (*type_ptr != END)
	{
		if (*type_ptr == STAR)
		{
			dir = opendir(".");
			if (dir == NULL)
				*token_ptr = ft_strdup("*");
			else
			{
				entries_array = (char **)ft_gc_malloc(sizeof(char *) * 1000);
				entries_count = 0;
				while ((entry = readdir(dir)) != NULL)
				{
					if (entry->d_name[0] != '.')
					{
						entries_array[entries_count] = ft_strdup(entry->d_name);
						entries_count++;
					}
				}
				old_tokens = line->token;
				old_types = line->type;
				line->token = (char **)ft_gc_malloc(sizeof(char *)
					* (original_count + entries_count));
				line->type = (t_token *)ft_gc_malloc(sizeof(t_token)
					* (original_count + entries_count));
				j = -1;
				while (++j < (token_ptr - old_tokens))
				{
					line->token[j] = old_tokens[j];
					line->type[j] = old_types[j];
				}
				token_ptr = line->token + j;
				type_ptr = line->type + j;
				i = -1;
				while (++i < entries_count)
				{
					*token_ptr++ = entries_array[i];
					*type_ptr++ = WORD;
				}
				j = (token_ptr - line->token) - 1;
				while (++j < original_count)
				{
					line->token[j + entries_count - 1] = old_tokens[j];
					line->type[j + entries_count - 1] = old_types[j];
				}
				line->token[original_count + entries_count - 1] = NULL;
				line->type[original_count + entries_count - 1] = END;
				closedir(dir);
			}
		}
		else
		{
			token_ptr++;
			type_ptr++;
		}
	}
	return (0);
}
