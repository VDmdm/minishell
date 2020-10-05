/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_search.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wquinoa <wquinoa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/20 16:35:34 by wquinoa           #+#    #+#             */
/*   Updated: 2020/07/27 17:48:03 by wquinoa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_print_error(char *str)
{
	if (ft_fput("%s", ft_strjoin_dlm("b42h: ", str, \
			": command not found\n"), 0, 2) == -2)
		ft_fput("b42h: %s: command not found\n", str, 0, 2);
}

static int	dir_search(t_shell *shell)
{
	DIR			*dirp;
	t_dirent	*entry;

	if (!(dirp = opendir(shell->cwd)))
		return (ft_perror(shell->cwd));
	while ((entry = readdir(dirp)))
	{
		ft_perror("readdir");
		if (ft_strcmp(entry->d_name, shell->split[0]))
			shell->cmd = ft_strjoin(shell->cwd, &shell->split[0][1]);
	}
	closedir(dirp);
	return (0);
}

static int	path_search(t_shell *shell)
{
	DIR			*dirp;
	t_dirent	*entry;
	char		**tmp;

	tmp = shell->path;
	while (tmp && *tmp)
	{
		if (!(dirp = opendir(*tmp)))
		{
			tmp++;
			continue ;
		}
		while ((entry = readdir(dirp)))
			if (!ft_strcmp(entry->d_name, shell->split[0]))
				if (!(shell->cmd = ft_strjoin_dlm(*tmp, "/", \
				shell->split[0])))
					ft_perror_exit("b42h");
		tmp++;
		closedir(dirp);
		if (shell->cmd)
			break ;
	}
	if (!shell->cmd)
		shell->cmd = ft_strdup(shell->split[0]);
	return (0);
}

void		search(t_shell *shell)
{
	int			pid;

	ft_del(shell->cmd);
	if ((pid = fork()) < 0)
		ft_perror("search");
	wait(&shell->status);
	if (WIFEXITED(shell->status))
		ft_switch_env(shell->envir, "?",
		ft_itoa((WEXITSTATUS(shell->status))));
	if (pid == 0)
	{
		if (ft_isalnum(shell->split[0][0]))
			path_search(shell);
		else if (!ft_strncmp("./", shell->split[0], 2))
			dir_search(shell);
		else
			shell->cmd = ft_strdup(shell->split[0]);
		ft_tabclear(shell->environ);
		if (!(shell->environ = ft_env_to_tab(shell->envir)))
			ft_perror_exit(shell->split[0]);
		execve(shell->cmd, shell->split, shell->environ);
		ft_print_error(shell->split[0]);
		prslst_free(shell->cmds);
		exit(127);
	}
}
