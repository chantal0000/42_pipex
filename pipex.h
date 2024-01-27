/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbuerge <chbuerge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 17:02:28 by chbuerge          #+#    #+#             */
/*   Updated: 2024/01/27 14:54:29 by chbuerge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/libft.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>

// PIPEX_UTILS.C
void	ft_error(char *str);
void	ft_error_after_pipe(char *str, int fd_array[2]);
void	execute(char **env, char **input, char *cmd, int fd_array[2]);
char	*get_path(char *cmd, char **env, int fd_array[2]);
char	*get_command_path(char *cmd, char **path, int fd_array[2]);

#endif
