/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbuerge <chbuerge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:17:47 by chbuerge          #+#    #+#             */
/*   Updated: 2024/01/30 14:19:45 by chbuerge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_error(char *str)
{
	ft_printf("%s", str);
	exit(1);
}

void	ft_error_after_pipe(char *str, int fd_array[2], int exit_status)
{
	ft_printf("%s", str);
	close(fd_array[0]);
	close(fd_array[1]);
	exit(exit_status);
}

void	free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/*
** 'execve' loads and exectues a new program referred to by
** the specific file path in the current process, replaces the current process
*/
int	execute(char **env, char **input, char *cmd, int fd_array[2])
{
	char	**split_cmd;
	char	*cmd_path;
	int		i;

	i = 0;
	split_cmd = ft_split(cmd, ' ');
	cmd_path = get_path(split_cmd[0], env, fd_array);
	if (!cmd_path)
	{
		write(STDERR_FILENO, "Command '", 9);
		while (cmd[i])
		{
			write(STDERR_FILENO, &cmd[i], 1);
			i++;
		}
		write(STDERR_FILENO, "' not found\n", 12);
		if (split_cmd)
			free_array(split_cmd);
		//exit(1);
		return (-1);
	}
	execve(cmd_path, split_cmd, env);
	// returns -1 if it fails -
	return (-1);
}

		// while (split_cmd[i])
		// {
		// 	free(split_cmd[i]);
		// 	i++;
		// }
		// free(split_cmd);
/*
** locate the absolute path of a given commmand by searching through the
** directories listed in the environment variable
*/
char	*get_path(char *cmd, char **env, int fd_array[2])
{
	char	**path;
	int		i;

	i = 0;
	// PATH doesnt exist what to do ?
	while (env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
		{
			path = ft_split(env[i] + 5, 58);
			break ;
		}
		i++;
	}
	 if (!path)
		return (NULL);
	return (get_command_path(cmd, path, fd_array));
}

/*
** join the path to one string and add the name of the command to create the
** whole cmd_path
*/
char	*get_command_path(char *cmd, char **path, int fd_array[2])
{
	char	*tmp_path;
	char	*cmd_path;
	int		i;

	i = 0;
	if (access(cmd, X_OK) == 0)
	{
	//	printf("cmd_path %s\n", cmd_path);
		return (cmd);
	}
	while (path[i])
	{
		tmp_path = ft_strjoin(path[i], "/");
		cmd_path = ft_strjoin(tmp_path, cmd);
		free(tmp_path);
		if (access(cmd_path, X_OK) == 0)
		{
		//	printf("cmd_path %s\n", cmd_path);
			return (cmd_path);
		}
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
