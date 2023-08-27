/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 14:32:35 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/27 16:14:39 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int is_special_char(char c)
// {
//     return (c == '|' || c == '>' || c == '<' || c == '&');
// }

static int	count_substrings(char *line)
{
	int	count;

	count = 0;
	while (*line && (*line == ' ' || *line == '\t'))
		line++;
	while (*line)
	{
		if (*line == '"' || *line == '\'')
		{
			count++;
			line++;
			while (*line && (*line != '"' && *line != '\''))
				line++;
			if (*line)
				line++;
		}
		else if (*line != ' ' && *line != '\t')
		{
			count++;
			while (*line && *line != ' ' && *line != '\t' && *line != '"' && *line != '\'')
				line++;
		}
		else
			line++;
	}
	return (count);
}

static char extract_quoted_string(char **line_pointer, char **output)
{
    char *line = *line_pointer;
    char quote = *line++;
    char *start = line - 1;
    int nbr_backslash = 0;
    while (*line && (*line != quote || *(line - 1) == '\\'))
    {
        if (*line == '\\' && *(line - 1) == '\\')
            nbr_backslash++;
        line++;
    }
    if (nbr_backslash % 2 == 1)
        line--;
    
    *output = ft_strndup(start, line - start + 1);
    *line_pointer = line + 1;
    
    return quote;
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

char **parsed_line(char *line)
{
    char *temp_line = line;
    char **result = (char **)ft_gc_malloc(sizeof(char *) * (count_substrings(line) + 2));
    if (!result)
        return NULL;
    int index = 0;
    while (*line)
    {
        while (*line && (*line == ' ' || *line == '\t'))
            line++;
        if ((*line == '"' || *line == '\'') && (line == temp_line || *(line - 1) != '\\'))
        {
            char *current_string;
            char quote_used = extract_quoted_string(&line, &current_string);
            while (*(line - 1) == quote_used && (*line != ' ' && *line != '\t' && *line != '\0'))
            {
                char *next_string;
                extract_quoted_string(&line, &next_string);
                char *merged_string = ft_strjoin(current_string, next_string);
                current_string = merged_string;
            }
            result[index++] = current_string;
        }
        else if (*line && *line != ' ' && *line != '\t')
            result[index++] = extract_unquoted_string(&line);
    }
    result[index] = NULL;
    return result;
}
