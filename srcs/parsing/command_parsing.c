/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:42:55 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/23 13:56:40 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

int	parse_cmd(t_global *global, t_line *line)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**new_tokens;
	int				new_count = 0, i = 0;
	char			**token_ptr;
	t_token			*type_ptr;
	char			*temp;

	(void) global;
	token_ptr = line->token;
	type_ptr = line->type;
	while (*type_ptr != END)
	{
		if (*type_ptr == STAR)
		{
			dir = opendir(".");
			if (dir == NULL)
			{
				*token_ptr = ft_strdup("*");
			}
			else
			{
				while ((entry = readdir(dir)) != NULL)
				{
					if (entry->d_name[0] != '.')
						new_count++;
				}
				rewinddir(dir);
				new_tokens = (char **)ft_gc_malloc(sizeof(char *) * (new_count + 1));
				while ((entry = readdir(dir)) != NULL)
				{
					if (entry->d_name[0] != '.')
					{
						new_tokens[i] = ft_strdup(entry->d_name);
						i++;
					}
				}
				new_tokens[i] = NULL;
				closedir(dir);
				temp = *token_ptr;
				*token_ptr = ft_strjoin(temp, new_tokens[0]);
				i = 1;
				while (new_tokens[i])
				{
					temp = *token_ptr;
					*token_ptr = ft_strjoin(temp, " ");
					temp = *token_ptr;
					*token_ptr = ft_strjoin(temp, new_tokens[i]);
					i++;
				}
			}
		}
		token_ptr++;
		type_ptr++;
	}
	return (0);
}
