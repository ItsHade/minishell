#include "../include/minishell.h"

extern int g_return;

int	quote_error(char *s)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (s[i])
	{
		if (s[i] && s[i] == '\'')
		{
			sq++;
			i++;
			while (s[i] && s[i] != '\'')
				i++;
			if (s[i] && s[i] == '\'')
				sq = 0;
		}
		else if (s[i] && s[i] == '\"')
		{
			dq++;
			i++;
			while (s[i] && s[i] != '\"')
				i++;
			if (s[i] && s[i] == '\"')
				dq = 0;
		}
		if (s[i])
			i++;
	}
	if (sq || dq)
		return (ft_putstr_fd("unquoted issues >\n", 2), ft_free(s),1);
	return (0);
}

int	syntax_error(t_token **tl)
{
	t_token *cur;

	cur = *tl;
	if (!cur)
	{
		return (EMPTY);
	}
	while (cur)
	{
		if ((cur->label == I_RDR || cur->label == O_RDR || cur->label == HEREDOC || cur->label == OAPPEND) && (!cur->next))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
			g_return = 2;
			return (1);
		}
		if ((cur->label == I_RDR || cur->label == O_RDR || cur->label == HEREDOC || cur->label == OAPPEND) && (cur->next->label != WORD))
			break;
		if (cur->label == PIPE && ((cur->prev && cur->prev->label != WORD)  || cur->next == NULL || cur->next->label == PIPE))
			break;
		if (cur->label == PIPE && cur->index == 0)
			break;
		if (cur->label == SYNTAX)
			break;
		cur = cur->next;
	}
	if (cur)
	{
		g_return = 2;
		printf("minishell: syntax error near unexpected token `%s'\n", (char *)cur->value);
		return (1);
	}
	return (0);
}
