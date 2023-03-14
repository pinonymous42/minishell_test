/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7_destructer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 01:40:47 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/13 16:03:06 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_token(t_token *token)
{
	if (token == NULL)
		return ;
	if (token->word)
		free(token->word);
	free_token(token->next);
	free(token);
}

void	free_array(char **array)
{
	int	i;

	if (array == NULL)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void free_list(t_environ *list)
{
	t_environ *tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		if (tmp)
			free(tmp);
	}
}