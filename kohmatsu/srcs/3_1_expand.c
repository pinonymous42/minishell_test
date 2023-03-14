/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_1_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 01:40:55 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/13 17:48:39 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcmp(char *str1, char *str2)
{
	if (str1 == NULL || str2 == NULL)
		return (1);
	while (*str1 && *str2 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	return (*str1 - *str2);
}

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += strlen(*s);
	new = malloc(sizeof(char) * size);
	if (new == NULL)
		function_error("malloc");
	if (*s)
		strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

int	is_variable_character(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	double_expansion(char **new_word, char **ret, t_environ *list)
{
	char	*var;

	var = ft_calloc(sizeof(char), 1);
	if (ft_strchr(*new_word, '$') || ft_strchr(*new_word, ' ')
		|| not_allowed_variant_character(*new_word))
	{
		while (**new_word != '$' && **new_word != ' '
			&& is_variable_character(**new_word))
		{
			append_char(&var, **new_word);
			*new_word += 1;
		}
	}
	else
		var = ft_strndup(*new_word, ft_strchr(*new_word, '\0') - *new_word);
	*ret = ft_strjoin_with_free(*ret, search_env(var, list), FIRST_PARAM);
	free(var);
}

void	double_moving(char **new_word)
{
	if (ft_strchr(*new_word, '$') || ft_strchr(*new_word, ' '))
	{
		if (ft_strchr(*new_word, ' ') == 0 || (ft_strchr(*new_word, '$')
				&& (ft_strchr(*new_word, '$') < ft_strchr(*new_word, ' '))))
			*new_word = ft_strchr(*new_word, '$');
		else if (ft_strchr(*new_word, '$') == 0 || (ft_strchr(*new_word, ' ')
				&& (ft_strchr(*new_word, '$') > ft_strchr(*new_word, ' '))))
			*new_word = ft_strchr(*new_word, ' ');
	}
	else
	{
		if (not_allowed_variant_character(*new_word))
		{
			while (is_variable_character(**new_word))
				*new_word += 1;
		}
		else
			*new_word = ft_strchr(*new_word, '\0');
	}
}

void	double_expand_status(char **ret, char **new_word)
{
	*ret = ft_strjoin_with_free(*ret, ft_itoa(g_signal.status), FIRST_PARAM);
	*new_word += 1;
}

char	*double_variable_expand(char *new_word, t_environ *list)
{
    char	*tmp;
    char	*ret;

	g_signal.do_split = 1;
    tmp = new_word;
    while (*new_word != '$')
        new_word++;
    if (new_word == tmp)
        ret = ft_calloc(sizeof(char), 1);
    else
        ret = ft_strndup(tmp, new_word - tmp);
    new_word++;
    if (*new_word == '?')
		double_expand_status(&ret, &new_word);
    else
    {
		double_expansion(&new_word, &ret, list);
		double_moving(&new_word);
    }
    ret = ft_strjoin_with_free(ret, new_word, FIRST_PARAM);
    free(tmp);
    return (ret);
}

void	remove_single_quote(char **p, char **new_word)
{
	*p += 1;
	while (**p != SINGLE_QUOTE)
	{
		if (**p == '\0')
			assert_error("Unclosed single quote");
		if (**p == '$')
			g_signal.not_expand_flag = 1;
		append_char(new_word, **p);
		*p += 1;
	}
	*p += 1;
}

void	remove_double_quote(char **p, char **new_word, t_environ *list)
{
	*p += 1;
	while (**p != DOUBLE_QUOTE)
	{
		if (**p == '\0')
			assert_error("Unclosed single quote");
		append_char(new_word, **p);
		*p += 1;
	}
	*p += 1;
	while (ft_strchr(*new_word, '$'))
		*new_word = double_variable_expand(*new_word, list);
}

void	expand_status(char **p, char **new_word)
{
	*p += 1;
	*new_word = ft_itoa(g_signal.status);
}

void	expansion(char **p, char **new_word, t_environ *list)
{
	char	*var;
	
	if (ft_strchr(*p, '$'))
		var = ft_strndup(*p, ft_strchr(*p, '$') - *p);
	else
	{
		if (not_allowed_variant_character(*p))
		{
			var = ft_calloc(sizeof(char), 1);
			while (is_variable_character(**p))
			{
				append_char(&var, **p);
				*p += 1;
			}
		}
		else
			var = ft_strndup(*p, ft_strchr(*p, '\0') - *p);
	}
	if (*new_word == NULL)
		*new_word = ft_strdup(search_env(var, list));
	else
		*new_word = ft_strjoin_with_free(*new_word, search_env(var, list), FIRST_PARAM);
	free(var);
}

void	expand_variable(char **p, char **new_word, t_environ *list)
{
	g_signal.do_split = 1;
	while (ft_strchr(*p, '$'))
	{
		*p += 1;
		if (**p == '?')
			expand_status(p, new_word);
		else
		{
			expansion(p, new_word, list);
			if (ft_strchr(*p, '$'))
				*p = ft_strchr(*p, '$');
			else
			{
				if (not_allowed_variant_character(*p))
				{
					while (is_variable_character(**p))
						*p += 1;
				}
				else
					*p = ft_strchr(*p, '\0');
			}
		}
	}
}

void	just_append(t_token *tok, char **p, char **new_word)
{
	if (ft_strcmp(tok->word, "<<") == 0 && ft_strchr(tok->next->word, '\"') && ft_strcmp(tok->next->next->word, "<<") == 1)
		g_signal.not_expand_flag = 1;
	append_char(new_word, **p);
	*p += 1;
}

void	quote_removal(t_token *tok, t_environ *list)
{
	char	*new_word;
	char	*p;

	if (tok == NULL || (tok->kind != TOKEN_WORD
			&& tok->kind != TOKEN_METACHAR) || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = NULL;
	while (*p)
	{
		if (*p == SINGLE_QUOTE)
			remove_single_quote(&p, &new_word);
		else if (*p == DOUBLE_QUOTE)
			remove_double_quote(&p, &new_word, list);
		else if (*p == '$')
			expand_variable(&p, &new_word, list);
		else
			just_append(tok, &p, &new_word);
	}
	free(tok->word);
	tok->word = new_word;
	quote_removal(tok->next, list);
}

void	writing_for_one(int num, char **array, t_environ *list, char *line)
{
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strcmp(line, array[num]) == 0)
		{
			free(line);
			break ;
		}
		if (g_signal.not_expand_flag == 0 && ft_strchr(line, '$'))
		{
			while (ft_strchr(line, '$'))
				line = double_variable_expand(line, list);
		}
		write(g_signal.heredoc_fd, line, ft_strlen(line));
		write(g_signal.heredoc_fd, "\n", 1);
		free(line);
	}
	dup2(g_signal.input_fd, 0);
	close(g_signal.input_fd);
	close(g_signal.heredoc_fd);
}

void	writing_for_not_one_v1(char *line, char **array, int num)
{
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strcmp(line, array[num]) == 0)
		{
			free(line);
			break ;
		}
		free(line);
	}
}

void	writing_for_not_one_v2(int num, char **array,
		t_environ *list, char *line)
{
	char	*tmp;

	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strcmp(line, array[num]) == 0)
		{
			free(line);
			break ;
		}
		if (g_signal.not_expand_flag == 0 && search_env(line + 1, list))
		{
			tmp = search_env(line + 1, list);
			free(line);
			line = tmp;
		}
		write(g_signal.heredoc_fd, line, ft_strlen(line));
		write(g_signal.heredoc_fd, "\n", 1);
		free(line);
	}
}

void	write_to_heredoc_one(char **array, int i, t_environ *list)
{
	char	*line;

	line = NULL;
	g_signal.heredoc_fd = open(".heredoc",
			(O_WRONLY | O_CREAT | O_TRUNC), 0644);
	if (g_signal.heredoc_fd == -1)
		function_error("open");
	heredoc_signal();
	writing_for_one(i + 1, array, list, line);
}

void	write_to_heredoc_not_one(char **array, int i,
		int *heredoc_flag, t_environ *list)
{
	char	*line;

	line = NULL;
	if (ft_strcmp(array[i + 4], "<<") == 0)
		return ;
	heredoc_signal();
	*heredoc_flag = 1;
	g_signal.heredoc_fd = open(".heredoc",
			(O_WRONLY | O_CREAT | O_TRUNC), 0644);
	if (g_signal.heredoc_fd == -1)
		function_error("open");
	writing_for_not_one_v1(line, array, i + 1);
	if (g_signal.other_code == FALSE)
		writing_for_not_one_v2(i + 3, array, list, line);
	dup2(g_signal.input_fd, 0);
	close(g_signal.input_fd);
	close(g_signal.heredoc_fd);
}

char	**expand(t_token *tok, t_environ *list)
{
	char	**array;
	int		heredoc_count;
	int		i;
	int		heredoc_flag;

	i = 0;
	quote_removal(tok, list);
	array = token_list_to_array(tok);
	heredoc_count = count_heredoc(array);
	heredoc_flag = 0;
	while (array[i])
	{
		if (ft_strcmp(array[i], "<<") == 0 && heredoc_flag == 0)
		{
			if (heredoc_count == 1)
				write_to_heredoc_one(array, i, list);
			else
				write_to_heredoc_not_one(array, i, &heredoc_flag, list);
		}
		i++;
	}
	return (array);
}
