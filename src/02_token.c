#include "../include/minishell.h"

int	ft_is_sep(char c, const char *sep)
{
	int	i;

	i = 0;
	while (sep[i])
	{
		if (c == sep[i])
			return (1);
		i++;
	}
	return (0);
}

int	ft_count_words(char *line, const char *sep, const char *meta)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (ft_next_word_len(line, &i, sep, meta) > 0)
			count++;
	}
	return (count);
}
/*
	int	in_quotes;
	int	len;
	char		m;
	char	quote;
	char	quotes[4] = "'\"";

	in_quotes = 0;
	len = 0;
	while (line[*i] && ft_is_sep(line[*i], sep))
		(*i)++;
	while (line[*i])
	{
		if (line[*i] && ft_is_sep(line[*i], meta) && in_quotes == 0 && len == 0)
		{
			m = line[*i];
			while (line[*i] && ft_is_sep(line[*i], meta) && in_quotes == 0 && line[*i] == m)
			{
				(*i)++;
				len++;
				if (m == '|')
					break;
			}
			break;
		}
*/

//need to split this for norm
int	ft_next_word_len(char *line, int *i, const char *sep, const char *meta)
{
	int	in_quotes;
	int	len;
	int		stacked;
	char	quote;
	const char	quotes[4] = "'\"";

	in_quotes = 0;
	len = 0;
	while (line[*i] && ft_is_sep(line[*i], sep))
		(*i)++;
	while (line[*i])
	{
		if (line[*i] && ft_is_sep(line[*i], meta) && in_quotes == 0 && len == 0)
		{
			stacked = 0;
			while (line[*i] && ft_is_sep(line[*i], meta) && in_quotes == 0 && stacked < 2)
			{
				if (line[*i] == '|')
					stacked = 3;
				(*i)++;
				len++;
				stacked++;
			}
			break;
		}
		else if (line[*i] && ft_is_sep(line[*i], meta) && in_quotes == 0 && len != 0)
			break;
		if (line[*i] && ft_is_sep(line[*i], quotes) && in_quotes == 0)
		{
			in_quotes = 1;
			quote = line[*i];
			(*i)++;
			len++;
		}
		if (line[*i] && in_quotes == 1)
		{
			if (line[*i] == quote)
				in_quotes = 0;
			(*i)++;
			len++;
		}
		if (line[*i] && !ft_is_sep(line[*i], sep) && !ft_is_sep(line[*i], meta)
			&& !ft_is_sep(line[*i], quotes) && in_quotes == 0)
		{
			(*i)++;
			len++;
		}
		if (line[*i] && ft_is_sep(line[*i], sep) && in_quotes == 0)
			break;
	}
	return (len);
}

char	*ft_get_next_word(char *line, int *i, const char *sep, const char *meta)
{
	char *word;
	int	a;
	int	len;

	a = 0;
	len = ft_next_word_len(line, i, sep, meta);
	word = ft_calloc(sizeof(char), (len + 1));
	if (!word)
		return (NULL);
	*i -= len;
	while (line[*i] && a < len)
	{
		word[a] = line[*i];
		a++;
		(*i)++;
	}
	word[a] = '\0';
	return (word);
}

int ft_get_token_list(t_token **token_list, char *line, const char *sep, const char *meta)
{
	int	nb_of_tokens;
	int	a;
	t_token *new;
	char *word;
	int	i;

	a = 0;
	i = 0;
	nb_of_tokens = ft_count_words(line, sep, meta);
	while (a < nb_of_tokens)
	{
		word = ft_get_next_word(line, &i, sep, meta);
		if (!word)
			return (-1);
		new = ft_lstnew((void *)word);
		if (!new)
			return (-1);
		ft_lstadd_back(token_list, new);
		a++;
	}
	return (0);
}

// echo "printing path: $PATH" | grep 'bin'>outfile
//
// echo lll"printing path>: $PATH"this |grep 'bin'>>outfile
//
// echo lll"printing 'path'>: $PATH"this |grep 'bin'>>'outf'">>"'ile'""test || ls
//
//

