/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:42:55 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/26 16:55:27 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	define_home_folder(t_env *env, t_global *global)
{
	if (env == NULL)
		return ;
	global->home_folder = get_env_value("HOME", env);
}

// int	parse_cmd(t_line *line, t_env *env, t_global *global)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	int				j;
// 	int				i;
// 	char			**token_ptr;
// 	t_token			*type_ptr;
// 	int				original_count;
// 	char			**entries_array;
// 	int				entries_count;
// 	char			**old_tokens;
// 	t_token			*old_types;
// 	int				token_idx;
// 	int				type_idx;

// 	token_idx = 0;
// 	type_idx = 0;
// 	original_count = 0;
// 	while (line->token[original_count] != NULL)
// 		original_count++;
// 	token_ptr = line->token;
// 	type_ptr = line->type;
// 	while (*type_ptr != END)
// 	{
// 		if (*type_ptr == STAR)
// 		{
// 			dir = opendir(".");
// 			if (dir == NULL)
// 				*token_ptr = ft_strdup("*");
// 			else
// 			{
// 				entries_array = (char **)ft_gc_malloc(sizeof(char *) * 1000);
// 				entries_count = 0;
// 				while ((entry = readdir(dir)) != NULL)
// 				{
// 					if (entry->d_name[0] != '.')
// 					{
// 						entries_array[entries_count] = ft_strdup(entry->d_name);
// 						entries_count++;
// 					}
// 				}
// 				old_tokens = line->token;
// 				old_types = line->type;
// 				line->token = (char **)ft_gc_malloc(sizeof(char *)
// 					* (original_count + entries_count));
// 				line->type = (t_token *)ft_gc_malloc(sizeof(t_token)
// 					* (original_count + entries_count));
// 				j = -1;
// 				while (++j < (token_ptr - old_tokens))
// 				{
// 					line->token[j] = old_tokens[j];
// 					line->type[j] = old_types[j];
// 				}
// 				token_ptr = line->token + j;
// 				type_ptr = line->type + j;
// 				i = -1;
// 				while (++i < entries_count)
// 				{
// 					*token_ptr++ = entries_array[i];
// 					*type_ptr++ = WORD;
// 				}
// 				j = (token_ptr - line->token) - 1;
// 				while (++j < original_count)
// 				{
// 					line->token[j + entries_count - 1] = old_tokens[j];
// 					line->type[j + entries_count - 1] = old_types[j];
// 				}
// 				line->token[original_count + entries_count - 1] = NULL;
// 				line->type[original_count + entries_count - 1] = END;
// 				closedir(dir);
// 			}
// 		}
// 		else if (*type_ptr == TILDE)
// 		{
// 			define_home_folder(env, global);
// 			*token_ptr = ft_strdup(get_env_value("HOME", env));
// 			if (*token_ptr == NULL)
// 				*token_ptr = ft_strdup("~");
// 			type_ptr++;
// 		}
// 		else
// 		{
// 			token_ptr++;
// 			type_ptr++;
// 		}
// 	}
// 	return (0);
// }


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
				t_token *new_types = (t_token *)ft_gc_malloc(sizeof(t_token) * (original_count + entries_count));

				// Copy old tokens/types until current index.
				for (int j = 0; j < token_idx; j++) {
					new_tokens[j] = global->line->token[j];
					new_types[j] = global->line->type[j];
				}

				// Add directory entries.
				for (int i = 0; i < entries_count; i++) {
					new_tokens[token_idx + i] = entries_array[i];
					new_types[type_idx + i] = WORD;
				}

				// Copy remaining old tokens/types.
				for (int j = token_idx+1; j < original_count; j++) { // start from token_idx+1 since * is replaced
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
