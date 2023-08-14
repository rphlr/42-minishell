/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:43:52 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/07 17:17:24 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*format_options(char *token)
{
	char	*new_token;
	char	*dst;
	char	*scan;

	new_token = ft_gc_malloc(sizeof(char) * (ft_strlen(token) + 1));
	if (!new_token)
		return (NULL);
	dst = new_token;
	while (*token)
	{
		scan = token + 1;
		while (*scan && *scan != *token)
			scan++;
		if (!*scan)
			*dst++ = *token;
		token++;
	}
	*dst = '\0';
	return (new_token);
}

int	check_options_doublon(char *token)
{
	char	*start;
	char	*compare;

	start = token;
	while (*start)
	{
		compare = start + 1;
		while (*compare)
		{
			if (*start == *compare)
				return (1);
			compare++;
		}
		start++;
	}
	return (0);
}

int	check_options_syntax(char *token)
{
	while (*++token)
	{
		if (!ft_isalpha(*token) && !ft_isdigit(*token))
			return (1);
	}
	return (0);
}
