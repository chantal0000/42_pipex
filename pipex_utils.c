/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbuerge <chbuerge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:17:47 by chbuerge          #+#    #+#             */
/*   Updated: 2024/01/25 15:19:52 by chbuerge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "pipex.h"

// function to handle error

void	ft_error(char *str)
{
	ft_printf("%s", str);
	exit(1);
}

/*
** locate the absolute path of a given commmand by searching through the
** directories listed in the environment variable
*/
char **get_path(char *cmd, char **env)
{
	char **path;
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
		{
			path = ft_split(env[i] + 5, 58);
			break;
		}
		i++;
	}
	i = 0;
	// while (path[i])
	// {
	// 	printf("%s\n", path[i]);
	// 	i++;
	// }
	return (path);
}
