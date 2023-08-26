/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 14:32:35 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/26 20:37:19 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_substrings(char *line)
{
	int	count;

	count = 0;
	while (*line)
	{
		if (*line == '"' || *line == '\'')
		{
			count++;
			line++;
			while (*line && (*line != '"' && *line != '\''))
				line++;
		}
		else if (*line != ' ')
		{
			count++;
			while (*line && *line != ' ' && *line != '"' && *line != '\'')
				line++;
		}
		else
			line++;
	}
	return (count);
}

static char	*extract_quoted_string(char **line_pointer)
{
	char	*line;
	char	quote;
	char	*start;
	int		nbr_backslash;

	line = *line_pointer;
	quote = *line++;
	start = line - 1;
	nbr_backslash = 0;
	while (*line && (*line != quote || *(line - 1) == '\\'))
	{
		if (*line == '\\' && *(line - 1) == '\\')
			nbr_backslash++;
		line++;
	}
	if (nbr_backslash % 2 == 1)
		line--;
	*line_pointer = line + 1;
	return (ft_strndup(start, line - start + 1));
}

static char	*extract_unquoted_string(char **line_pointer)
{
	char	*line;
	char	*start;

	line = *line_pointer;
	start = line;
	while (*line && *line != ' ' && *line != '"' && *line != '\'')
		line++;
	*line_pointer = line;
	return (ft_strndup(start, line - start));
}

char	**parsed_line(char *line)
{
	char	*temp_line;
	char	**result;
	int		index;

	temp_line = line;
	result = (char **)ft_gc_malloc(sizeof(char *) * (count_substrings(line) + 2));
	if (!result)
		return (NULL);
	index = 0;
	while (*line)
	{
		if ((*line == '"' || *line == '\'') && (line == temp_line || *(line
					- 1) != '\\'))
			result[index++] = extract_quoted_string(&line);
		else if (*line == '\t' || *line == ' ')
		{
			line++;
			continue ;
		}
		else
			result[index++] = extract_unquoted_string(&line);
	}
	result[index] = NULL;
	return (result);
}
