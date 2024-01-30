/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbuerge <chbuerge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 18:11:09 by chbuerge          #+#    #+#             */
/*   Updated: 2024/01/30 18:11:18 by chbuerge         ###   ########.fr       */
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
