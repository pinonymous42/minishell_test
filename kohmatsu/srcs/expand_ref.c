/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ref.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 01:40:55 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/26 18:02:47 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += strlen(*s);
	new = malloc(size);
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

char    *double_variable_expand(char *new_word, t_environ *list)
{
    char    *tmp;
    char    *ret;
    char    *var;

    tmp = new_word;
    while (*new_word != '$')
        new_word++;
    if (new_word == tmp)
        ret = ft_calloc(sizeof(char), 1);
    else
        ret = ft_strndup(tmp, new_word - tmp);
    new_word++;
    if (*new_word == '?')
    {
        ret = ft_strjoin_with_free(ret, ft_itoa(g_signal), FIRST_PARAM);
        new_word++;
    }
    else
    {
		if (ft_strchr(new_word, '$') || ft_strchr(new_word, ' '))
		{
			if (ft_strchr(new_word, ' ') == 0 || (ft_strchr(new_word, '$') && (ft_strchr(new_word, '$') < ft_strchr(new_word, ' '))))
				var = ft_strndup(new_word, ft_strchr(new_word, '$') - new_word);
			else if (ft_strchr(new_word, '$') == 0 || (ft_strchr(new_word, ' ') && (ft_strchr(new_word, '$') > ft_strchr(new_word, ' '))))
			{
				// printf("%s, %d\n", __FILE__, __LINE__);
				var = ft_strndup(new_word, ft_strchr(new_word, ' ') - new_word);
			}
		}
        else
            var = ft_strndup(new_word, ft_strchr(new_word, '\0') - new_word);
		// printf("{%s}\n", var);
        ret = ft_strjoin_with_free(ret, search_env(var, list), FIRST_PARAM);
		// printf("|%s|\n", ret);
        free(var);
		if (ft_strchr(new_word, '$') || ft_strchr(new_word, ' '))
		{
			if (ft_strchr(new_word, ' ') == 0 || (ft_strchr(new_word, '$') && (ft_strchr(new_word, '$') < ft_strchr(new_word, ' '))))
				new_word = ft_strchr(new_word, '$');
			else if (ft_strchr(new_word, '$') == 0 || (ft_strchr(new_word, ' ') && (ft_strchr(new_word, '$') > ft_strchr(new_word, ' '))))
				new_word = ft_strchr(new_word, ' ');
		}
        else
            new_word = ft_strchr(new_word, '\0');
    }
    ret = ft_strjoin_with_free(ret, new_word, FIRST_PARAM);
    free(tmp);
    return (ret);
}

void	quote_removal(t_token *tok, t_environ *list)
{
	char	*new_word;
	char	*p;
	char	*var;

	if (tok == NULL || (tok->kind != TOKEN_WORD && tok->kind != TOKEN_METACHAR) || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = NULL;
	while (*p)
	{
		// printf("|%s|\n", p);
		if (*p == SINGLE_QUOTE)
		{
			p++;
			while (*p != SINGLE_QUOTE)
			{
				if (*p == '\0')
					assert_error("Unclosed single quote");
				append_char(&new_word, *p++);
			}
			p++;
		}
		else if (*p == DOUBLE_QUOTE)
		{
			p++;
			while (*p != DOUBLE_QUOTE)
			{
				if (*p == '\0')
					assert_error("Unclosed single quote");
				append_char(&new_word, *p++);
			}
			p++;
			// printf("%s\n", new_word);
			// exit(1);
			// int count = 0;
			while (ft_strchr(new_word, '$'))
			{
				// count++;
				new_word = double_variable_expand(new_word, list);
				// printf("%s, %d\n", __FILE__, __LINE__);
				// printf("[%s]\n", new_word);
				// if (count == 2)
				// 	exit(1);
			}
		}
		else if (*p == '$')//$VAR="echo hello"と入っていても後でどうせ中身をsplitするからただgetenvだけでいいんじゃね
        {
            while (ft_strchr(p, '$'))
            {
                p++;
                if (*p == '?')
                {
                    p++;
                    new_word = ft_itoa(g_signal);
                }
                else
                {
                    if (ft_strchr(p, '$'))
                        var = ft_strndup(p, ft_strchr(p, '$') - p - 1);
                    else
                        var = ft_strndup(p, ft_strchr(p, '\0') - p);
                    if (new_word == NULL)
                        new_word = ft_strdup(search_env(var, list));
                    else
                        new_word = ft_strjoin_with_free(new_word, search_env(var, list), FIRST_PARAM);
                    free(var);
                    if (ft_strchr(p, '$'))
                        p = ft_strchr(p, '$');
                    else
                        p = ft_strchr(p, '\0');
                }
            }
        }
		else
			append_char(&new_word, *p++);
	}
	free(tok->word);
	tok->word = new_word;
	quote_removal(tok->next, list);
}

void	expand(t_token *tok, t_environ *list)
{
	quote_removal(tok, list);
}