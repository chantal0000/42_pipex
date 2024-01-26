/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbuerge <chbuerge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:40:32 by chbuerge          #+#    #+#             */
/*   Updated: 2024/01/26 13:46:48 by chbuerge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** function handles the first command
** -> dup2 duplicates fd's so that the standard input/output of one cmd is
** connected to the read/write end of the pipe
** STDIN_FILENO is defined as '0' <-
** the 1st dup2 takes input from fd which is my input file.
** the 2nd dup2 redirects the standard output of child to the write end of pipe
*/
void	handle_cmd1(char **env, char **input, int fd_array[2])
{
	int		fd;
	char	*cmd;

	cmd = input[2];
	fd = open(input[1], O_RDONLY);
	if (fd == -1)
	{
		ft_printf("pipex: %s: No such file or directory \n", input[1]);
		ft_error("");
	}
	dup2(fd, STDIN_FILENO);
	dup2(fd_array[1], STDOUT_FILENO);
	close(fd_array[0]);
	execute(env, input, cmd, fd_array);
}

/*
** 0 file type & special permission, regular file in our case
** 644 permissions
** O_TRUNC -> no effect if file does not exist otherwise truncs file
** O_CREAT -> if file does not exist it will be created
** O_RDWR -> open file for reading and writing
*/
void	handle_cmd2(char **env, char **input, int fd_array[2])
{
	int		fd;
	char	*cmd;

	cmd = input[3];
	fd = open(input[4], O_TRUNC | O_CREAT | O_RDWR, 0644);
	dup2(fd, STDOUT_FILENO);
	dup2(fd_array[0], STDIN_FILENO);
	close(fd_array[1]);
	execute(env, input, cmd, fd_array);
}

// open argv[1] only reading? & argv[4] allowing everything?
// open the infile argv[1] -> read only permission
// open/ create outfile argv[4] -> more permission?
int	main(int argc, char **argv, char **env)
{
	int	fd_array[2];
	int	pid1;

	if (argc != 5)
		ft_error("./pipex infile cmd1 cm2 outfile\n");
	if (!env || !*env)
		ft_error("Error: env\n");
	if (pipe(fd_array) == -1)
		ft_error("pipe\n");
	pid1 = fork();
	if (pid1 < 0)
		ft_error("fork\n");
	if (pid1 == 0)
	{
		handle_cmd1(env, argv, fd_array);
	}
	waitpid(pid1, NULL, 0);
	handle_cmd2(env, argv, fd_array);
	return (0);
}
