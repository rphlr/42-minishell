/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 07:34:38 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/10 18:41:32 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*ignore_quotes(const char *str)
// {
// 	int		i;
// 	char	*result;

// 	i = 0;
// 	result = (char *)ft_gc_malloc((ft_strlen(str) + 1) * sizeof(char));
// 	if (!result)
// 		return (NULL);
// 	while (*str)
// 	{
// 		if (*str != '\'' && *str != '\"')
// 			result[i++] = *str;
// 		str++;
// 	}
// 	result[i] = '\0';
// 	return (result);
// }

// int	check_syntax(char *str)
// {
// 	int	nbr_of_simple_cote;
// 	int	nbr_of_double_cote;

// 	nbr_of_simple_cote = 0;
// 	nbr_of_double_cote = 0;
// 	while (*str)
// 	{
// 		if (*str == '\'')
// 			nbr_of_simple_cote++;
// 		if (*str == '\"')
// 			nbr_of_double_cote++;
// 	}
// 	if (nbr_of_simple_cote % 2 != 0 || nbr_of_double_cote % 2 != 0)
// 		return (0);
// 	return (1);
// }

int	quote_type(int type)
{
	if (type == CLOSED_DQUOTE)
		return (1);
	if (type == CLOSED_QUOTE)
		return (2);
	return (0);
}

char	*get_quote_content(char *str)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = (char *)ft_gc_malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'')
			i++;
		if (str[i] == '\'')
			break;
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*get_dquote_content(char *str)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = (char *)ft_gc_malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\"')
			i++;
		if (str[i] == '\"')
			break;
		if (str[i] == '\\')
		{
			if (str[i + 1] == '"')
				i++;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

void	echo_print(char *str, t_global *global)
{
	int type_of_quote;

	type_of_quote = quote_type((int) *global->cmd->type++);
	if (!type_of_quote)
	{
		// str = echo_content(str);
		// ft_printf("%c", *str++);
	}
	else if (type_of_quote == 1)
	{
		str = get_dquote_content(str);
		while (*str)
		{
			if (*str == '\\')
			{
				str++;
				if (*str == 'n')
					ft_printf("\n");
				else if (*str == 't')
					ft_printf("\t");
				else if (*str == 'v')
					ft_printf("\v");
				else if (*str == 'b')
					ft_printf("\b");
				else if (*str == 'r')
					ft_printf("\r");
				else if (*str == 'f')
					ft_printf("\f");
				else if (*str == 'a')
					ft_printf("\a");
				else if (*str == '0')
					ft_printf("\0");
				else if (*str == '\\')
					ft_printf("\\");
				else
					ft_printf("\\%c", *str);
				str++;
			}
			if (*str == '$')
			{
				str++;
				if (*str == '?')
					ft_printf("%d", global->exit_code);
				else if (*str == '$')
					ft_printf("%d", global->pid);
				else if (*str == '0')
					ft_printf("%s", global->cmd->token[0]);
				else if (*str == '_')
					ft_printf("%s", global->cmd->token[0]);
				else
					ft_printf("$%c", *str);
				str++;
			}
			ft_printf("%c", *str++);
		}
		ft_printf("%c", *str++);
	}
	while (*str)
	{
		ft_printf("%c", *str++);
	}
}

int	check_options(t_cmd *cmd)
{
	int	i;

	while (*cmd->token && *cmd->type == OPTIONS)
	{
		i = 0;
		while ((*cmd->token)[i])
		{
			if (i != 0 && (*cmd->token)[i] != 'n' && (*cmd->token)[i] != 'e')
				return (-1);
			i++;
		}
		cmd->token++;
		cmd->type++;
	}
	return (0);
}

t_option	get_echo_arg(t_cmd *cmd)
{
	t_token		*tmp_type;
	char		**tmp_token;

	tmp_type = cmd->type;
	tmp_token = cmd->token;
	if (*++cmd->token && *++cmd->type != OPTIONS)
		return (NO_OPTION);
	cmd->token--;
	cmd->type--;
	while (*++cmd->token && *++cmd->type == OPTIONS)
	{
		if (check_options(cmd) == -1)
		{
			while (*cmd->token && *cmd->type == OPTIONS)
			{
				*cmd->type = WORD;
				cmd->type++;
				cmd->token++;
			}
			cmd->token = tmp_token;
			cmd->type = tmp_type;
		}
		cmd->token++;
		printf("token : %s\n", *cmd->token);
		printf("next token : %s\n", *(cmd->token + 1));
		// types
		printf("type : %d\n", *cmd->type);
		printf("next type : %d\n", *(cmd->type + 1));
		printf("next next type : %d\n", *(cmd->type + 2));
		cmd->type = remove_double_option(cmd->token, cmd->type);
		*cmd->token = format_options(*cmd->token);
		printf("token : %s\n", *cmd->token);
		printf("next token : %s\n", *(cmd->token + 1));
		if (!ft_strcmp(*cmd->token, "-n"))
		{
			cmd->token++;
			cmd->type++;
			return (OPTION_N);
		}
		else if (!ft_strcmp(*cmd->token, "-e"))
		{
			*cmd->token = format_options(*cmd->token);
			cmd->token++;
			cmd->type++;
			return (OPTION_E);
		}
		else if (!ft_strcmp(*cmd->token, "-ne") || !ft_strcmp(*cmd->token, "-en"))
		{
			cmd->token++;
			cmd->type++;
			return (OPTION_NE);
		}
		cmd->token++;
		cmd->type++;
	}
	return (0);
}

int	valid_arg(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] != 'n' && str[i] != 'e' && str[i] != '-')
		{
			// printf("c not correct = %c\n", str[i]);
			return (1);
		}
	}
	return (0);
}

void	ft_echo(t_global *global)
{
	t_option	option_type;

	option_type = get_echo_arg(global->cmd);
	while (*global->cmd->token)
	{
		if (option_type == NO_OPTION)
		{
			while (*global->cmd->token)
			{
				echo_print(*global->cmd->token, global);
				global->cmd->token++;
				if (*global->cmd->token)
					ft_printf(" ");
			}
			ft_printf("\n");
		}
		else if (option_type == OPTION_N)
		{
			echo_print(*global->cmd->token++, global);
			if (*global->cmd->token)
				ft_printf(" ");
		}
		else if (option_type == OPTION_E)
		{
			while (*global->cmd->token)
			{
				echo_print(*global->cmd->token, global);
				global->cmd->token++;
				if (*global->cmd->token)
					ft_printf(" ");
			}
			ft_printf("\n");
		}
		else if (option_type == OPTION_NE)
		{
			while (*global->cmd->token)
			{
				echo_print(*global->cmd->token, global);
				global->cmd->token++;
				if (*global->cmd->token)
					ft_printf(" ");
			}
		}
		else if (option_type == INVALID_OPTION)
		{
			while (*global->cmd->token)
			{
				while (!valid_arg(*global->cmd->token++) && *global->cmd->type++ == OPTIONS)
					;
				global->cmd->token--;
				echo_print(*global->cmd->token, global);
				global->cmd->token++;
				if (*global->cmd->token)
					ft_printf(" ");
			}
			ft_printf("\n");
		}
	}
}
