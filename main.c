/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbuerge <chbuerge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 16:50:01 by chbuerge          #+#    #+#             */
/*   Updated: 2024/01/26 11:24:43 by chbuerge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//pipe
//create fork
// one proecess handles first command - child
// dup2
//execve
// send output with pipe to second process
// second process handles the second command using the output
	// of process1 (child?)

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
//ft_stderr
//ft_stdout
//ft_stdin

/*
** 'execve' loads and exectues a new program referred to by
** the specific file path in the current process, replaces the current process
*/
void	execute(char **env, char **input, char *cmd, int fd_array[2])
{
	char	**split_cmd;
	char	*cmd_path;

	split_cmd = ft_split(cmd, ' ');
	cmd_path = get_path(split_cmd[0], env);
	execve(cmd_path, split_cmd, env);
	// handle error?
}

/*
** function handles the first command
** -> dup2 duplicates fd's so that the standard input/output of one cmd is
** connected to the read/write end of the pipe
** STDIN_FILENO is defined as '0' <-
*/
void	child_process(char **env, char **input, int fd_array[2])
{
	int fd;
	char *cmd = input[2];
	fd = open(input[1], O_RDONLY);
	// take input from fd which is my input file
	dup2(fd, STDIN_FILENO);
	// redirects the standard output of child to the write end of the pipe
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
	int	fd;
	char	*cmd = input[3];
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
//	char *path;
	int fd_array[2];
	if (argc != 5)
		ft_error("./pipex infile cmd1 cm2 outfile\n");
	if (!env || !*env)
		ft_error("Error: env\n");
	if (pipe(fd_array) == -1)
		ft_error("pipe\n");
	int pid1 = fork();

	if (pid1 < 0)
		ft_error("fork\n");
	if (pid1 == 0)
	{
		// handle cmd1
		child_process(env, argv, fd_array);
	}
	waitpid(pid1, NULL, 0);
	//parent / cmd2 handle
	handle_cmd2(env, argv, fd_array);
	return (0);
}
