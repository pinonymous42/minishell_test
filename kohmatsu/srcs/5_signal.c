/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_signal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 12:54:49 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/14 11:35:49 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	signal_handler(int sig)
{
	(void)sig;
	g_signal.status = 1;
	g_signal.other_code = TRUE;
	rl_on_new_line();
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_redisplay();
}

int	set_signal(void)
{
	signal(SIGINT, &signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

int	set_signal_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	return (0);
}

int	set_signal_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

void	heredoc_signal_handler(int signal)
{
	(void)signal;
	g_signal.status = 1;
	g_signal.other_code = TRUE;
	close(0);
	write(1, "\n", 1);
}

int	heredoc_signal(void)
{
	signal(SIGINT, &heredoc_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
