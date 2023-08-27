/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:42:55 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/27 15:16:31 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	define_home_folder(t_env *env, t_global *global)
{
	if (env == NULL)
		return ;
	global->home_folder = get_env_value("HOME", env);
}

t_global	*parse_cmd(t_global *global)
{
	DIR				*dir;
	struct dirent	*entry;
	int				token_idx;
	int				type_idx;
	int				original_count;

	original_count = 0;
	while (global->line->token[original_count] != NULL)
		original_count++;
	token_idx = 0;
	type_idx = 0;
	while (global->line->type[type_idx] != END)
	{
		if (global->line->type[type_idx] == STAR)
		{
			dir = opendir(".");
			if (dir == NULL)
			{
				global->line->token[token_idx] = ft_strdup("*");
				token_idx++;
			}
			else
			{
				int entries_count = 0;
				char *entries_array[1000];
				while ((entry = readdir(dir)) != NULL) {
					if (entry->d_name[0] != '.' && entries_count < 1000) {
						entries_array[entries_count] = ft_strdup(entry->d_name);
						entries_count++;
					}
				}
				closedir(dir);
				char **new_tokens = (char **)ft_gc_malloc(sizeof(char *) * (original_count + entries_count));
				if (!new_tokens)
					return NULL;
				t_token *new_types = (t_token *)ft_gc_malloc(sizeof(t_token) * (original_count + entries_count));
				if (!new_types)
					return NULL;
				for (int j = 0; j < token_idx; j++) {
					new_tokens[j] = global->line->token[j];
					new_types[j] = global->line->type[j];
				}
				for (int i = 0; i < entries_count; i++) {
					new_tokens[token_idx + i] = entries_array[i];
					new_types[type_idx + i] = WORD;
				}
				for (int j = token_idx+1; j < original_count; j++) {
					new_tokens[j + entries_count - 1] = global->line->token[j];
					new_types[j + entries_count - 1] = global->line->type[j];
				}

				new_tokens[original_count + entries_count - 1] = NULL;
				new_types[original_count + entries_count - 1] = END;

				global->line->token = new_tokens;
				global->line->type = new_types;

				token_idx += entries_count;
			}
			type_idx++;
		} else if (global->line->type[type_idx] == TILDE) {
			define_home_folder(global->env, global);
			global->line->token[token_idx] = ft_strdup(get_env_value("HOME", global->env));
			if (global->line->token[token_idx] == NULL) global->line->token[token_idx] = ft_strdup("~");
			token_idx++;
			type_idx++;
		} else {
			token_idx++;
			type_idx++;
		}
	}
	global->line->cmds = init_cmds(global->line->token, global->line->type);
	return (global);
}