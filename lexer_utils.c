/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elchakir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 00:04:20 by elchakir          #+#    #+#             */
/*   Updated: 2024/08/10 00:04:31 by elchakir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_v2(const char **p, char *special, TokenType *type)
{
	if (**p == '>')
	{
		if (*(*p + 1) == '>')
		{
			special[0] = '>';
			special[1] = '>';
			special[2] = '\0';
			*type = TOKEN_APPEND_OUT;
			(*p)++;
		}
		else if (**p == '>' && *(*p + 1) != '>')
			*type = TOKEN_REDIRECT_OUT;
	}
}

void	handle_special_characters(const char **p, Token *tokens,
		int *num_tokens)
{
	TokenType	type;
	char		special[4];

	type = TOKEN_WORD;
	if (**p == '|' || **p == '<' || **p == '>' || **p == '&' || **p == '#')
	{
		special[0] = **p;
		special[1] = '\0';
		type = TOKEN_WORD;
		if (**p == '|' && *(*p + 1) != '|')
			type = TOKEN_PIPE;
		else if (**p == '|' && *(*p + 1) == '|')
			type = TOKEN_OR;
		else if (**p == '>')
			handle_v2(p, special, &type);
		else
			handel_else(p, special, &type);
		add_token(tokens, num_tokens, type, special);
		(*p)++;
		return ;
	}
	else
		handel_q(p, type, tokens, num_tokens);
}

int	splited(Token *tokens, int *num_tokens, QuoteWordParserState *state)
{
	int	i;

	i = 0;
	if (state->double_quotes || check_space(state->expanded_value) == 0)
	{
		while (state->expanded_value[i] && state->start[0] != '?')
			state->buffer[state->buffer_index++] = state->expanded_value[i++];
		state->buffer[state->buffer_index] = '\0';
		free(state->expanded_value);
	}
	else if (!state->double_quotes)
	{
		ft_buffer_split(tokens, num_tokens, state->expanded_value);
		return (free(state->expanded_value), 1);
	}
	return (0);
}
