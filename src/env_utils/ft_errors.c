/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wquinoa <wquinoa@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 22:54:55 by wquinoa           #+#    #+#             */
/*   Updated: 2020/07/26 22:35:50 by wquinoa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_perror(char *str)
{
	if (errno)
	{
		perror(str);
		errno = 0;
		return (-1);
	}
	if (!ft_switch_env(g_shell.envir, "?", ft_itoa(0)))
		return (ft_perror_exit("malloc"));
	return (0);
}

int		ft_perror_exit(char *str)
{
	if (errno)
	{
		perror(str);
		errno = 0;
		exit(1);
		return (-1);
	}
	return (0);
}

void	ft_ignore(int signal)
{
	if (signal == SIGINT)
	{
		if (g_shell.pid > 0)
			kill(g_shell.pid, 9);
		else if (!g_shell.cmds && g_shell.pid != 0)
		{
			ft_putstr_fd("\n", 1);
			ft_fput(PROMPT, SHELL, g_shell.cwd, 1);
		}
	}
	else if (signal == SIGQUIT && g_shell.cmds && g_shell.pid != 0)
		ft_putendl_fd("Quit: 3", 2);
}
