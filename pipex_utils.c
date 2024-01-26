/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbuerge <chbuerge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:17:47 by chbuerge          #+#    #+#             */
/*   Updated: 2024/01/26 13:49:51 by chbuerge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_error(char *str)
{
	ft_printf("%s", str);
	exit(1);
}

/*
** 'execve' loads and exectues a new program referred to by
** the specific file path in the current process, replaces the current process
*/
void	execute(char **env, char **input, char *cmd, int fd_array[2])
{
	char	**split_cmd;
	char	*cmd_path;
	int		i;

	i = 0;
	split_cmd = ft_split(cmd, ' ');
	cmd_path = get_path(split_cmd[0], env);
	//execve(cmd_path, split_cmd, env);
	if (execve(cmd_path, split_cmd, env) == -1)
	{
		write(STDERR_FILENO, "Command '", 9);
		while (cmd[i])
		{
			write(STDERR_FILENO, &cmd[i], 1);
			i++;
		}
		write(STDERR_FILENO, "' not found\n", 12);
		ft_error("");
	}
}

/*
** locate the absolute path of a given commmand by searching through the
** directories listed in the environment variable
*/
char	*get_path(char *cmd, char **env)
{
	char	**path;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
		{
			path = ft_split(env[i] + 5, 58);
			break ;
		}
		i++;
	}
	return (get_command_path(cmd, path));
}

/*
** join the path to one string and add the name of the command to create the
** whole cmd_path
*/
char	*get_command_path(char *cmd, char **path)
{
	char	*tmp_path;
	char	*cmd_path;
	int		i;

	i = 0;
	while (path[i])
	{
		tmp_path = ft_strjoin(path[i], "/");
		cmd_path = ft_strjoin(tmp_path, cmd);
		free(tmp_path);
		//printf("path, before access check: %s\n", cmd_path);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
	return (NULL);
}
