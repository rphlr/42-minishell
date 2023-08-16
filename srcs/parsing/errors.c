/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:45:19 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/15 17:06:20 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_state	*get_error_table(void)
{
	t_state	*error_table;

	error_table = ft_gc_malloc(8 * sizeof(t_state));
	if (!error_table)
		return (NULL);
	error_table[PIPE] = PIPE_ERROR;
	error_table[INPUT] = INPUT_ERROR;
	error_table[OUTPUT] = OUTPUT_ERROR;
	error_table[APPEND] = APPEND_ERROR;
	error_table[HEREDOC] = HEREDOC_ERROR;
	error_table[AND] = AND_ERROR;
	error_table[OR] = OR_ERROR;
	error_table[SEMICOLON] = SEMICOLON_ERROR;
	return (error_table);
}

t_state	ft_error(t_token *type, char **tokens)
{
	int		nbr_quote;
	int		nbr_dquote;
	t_state	*error_table;
	t_state	token_check_result;

	nbr_quote = 0;
	nbr_dquote = 0;
	error_table = get_error_table();
	token_check_result = check_token_errors(type, tokens, error_table);
	if (token_check_result != VALID)
		return (token_check_result);
	while (*type != END)
	{
		nbr_quote += (*type == NOT_CLOSED_QUOTE) + 2 * (*type == CLOSED_QUOTE);
		nbr_dquote += (*type == NOT_CLOSED_DQUOTE) + 2
			* (*type == CLOSED_DQUOTE);
		type++;
		tokens++;
	}
	if (nbr_quote % 2 != 0)
		return (QUOTE_ERROR);
	if (nbr_dquote % 2 != 0)
		return (DQUOTE_ERROR);
	return (VALID);
}
