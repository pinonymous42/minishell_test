/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 12:54:49 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/02/27 15:33:40 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    signal_handler(int sig)
{
    g_signal.status = 1;
    rl_on_new_line();
    rl_replace_line("", 0);
    write(1, "\n", 1);
    rl_redisplay();
}

int set_signal()
{
    signal(SIGINT, &signal_handler);
    signal(SIGQUIT, SIG_IGN);
}

int set_signal_child()
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    return (0);
}

int set_signal_parent()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    return (0);
}

// void    heredoc_signal_handler(int signal)
// {
//     // printf("%s, %d\n", __FILE__, __LINE__);
//     (void)signal;
//     write(1, "\n", 1);
//     g_signal.status = 1;
//     // printf("%d\n", g_signal.status);
// }

// int    heredoc_signal(void)
// {
//     signal(SIGINT, &heredoc_signal_handler);
//     signal(SIGQUIT, SIG_DFL);
//     return (0);
// }