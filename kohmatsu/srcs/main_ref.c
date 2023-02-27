/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_ref.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:19:10 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/26 18:01:22 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int count_argv(char **argv)
{
    int len;

    len = 0;
    while (*argv)
    {
        len++;
        argv++;
    }
    return (len);
}

void process_line(char *line, t_environ *list)
{
    t_token *token;
	char	**array;
	int		argc;
	bool	syntax_error;

	syntax_error = false;
    token = tokenize(line, &syntax_error);
    // while (token)
    // {
    //     printf("%s\n", token->word);
    //     token = token->next;
    // }
    // exit(1);
    if (token->kind == TOKEN_EOF)
        ;
    // else if (syntax_error == true)
    //     *status = TOKENIZE_ERROR;
    else
    {
		expand(token, list);
        // printf("%s, %d\n", __FILE__, __LINE__);
        // while (token)
        // {
        //     printf("%s\n", token->word);
        //     token = token->next;
        // }
        // exit(1);
		array = token_list_to_array(token);
		argc = count_argv(array);
		pipex(argc, array, list);
		free_array(array);
    }
    unlink("./.heredoc");
    free_token(token);
}

int main(int argc, char **argv, char **envp)
{
    char *line;
    t_environ *list;

    g_signal = 0;
    list = make_environ(envp);
    while (1)
    {
        // printf("|%d|\n", g_signal);
        set_signal();
        line = readline("minishell$ ");
        if (line == NULL)
            break;
        if (*line != '\0')
            add_history(line);
        process_line(line, list);
        free(line);
    }
    return (0);
}