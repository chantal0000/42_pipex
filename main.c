/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbuerge <chbuerge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:40:32 by chbuerge          #+#    #+#             */
/*   Updated: 2024/01/27 18:46:34 by chbuerge         ###   ########.fr       */
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
		ft_printf("pipex: %s: No such file or directory\n", input[1]);
		ft_error_after_pipe("", fd_array, 2);
	}
	dup2(fd, STDIN_FILENO);
	// close(fd);
	dup2(fd_array[1], STDOUT_FILENO);
	close(fd_array[0]);
	if (execute(env, input, cmd, fd_array) == -1)
	{
		//write(STDERR_FILENO, "-1\n", 3);
		close(fd_array[0]);
		close(fd_array[1]);
		close(fd);
		exit(1);
		//exit here?
	}
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
	if (fd == -1)
	{
		// should be different error message
		ft_printf("pipex: %s: Permission denied\n", input[1]);
		// close(fd_array[0]);
		// close(fd_array[1]);
		ft_error_after_pipe("", fd_array, 1);
		//ft_error("");
	}
	dup2(fd, STDOUT_FILENO);
	//close(fd);
	dup2(fd_array[0], STDIN_FILENO);
	close(fd_array[1]);
	//execute(env, input, cmd, fd_array);
	if (execute(env, input, cmd, fd_array) == -1)
	{
		//write(STDERR_FILENO, "-1\n", 3);
		close(fd_array[0]);
		close(fd_array[1]);
		close(fd);
		exit(1);
		//exit here?
	}
}

// open argv[1] only reading? & argv[4] allowing everything?
// open the infile argv[1] -> read only permission
// open/ create outfile argv[4] -> more permission?
int	main(int argc, char **argv, char **env)
{
	int	fd_array[2];
	int	id1;
	int	id2;
	int wstatus1;
	int wstatus2;

	if (argc != 5)
		ft_error("./pipex infile cmd1 cm2 outfile\n");
	if (!env || !*env)
		ft_error("Error: env\n");
		// ab hier muss ich pipe auch bei error schliessen
	if (pipe(fd_array) == -1)
		ft_error("pipe\n");
	id1 = fork();
	if (id1 < 0)
		ft_error_after_pipe("fork\n", fd_array, 1);
	if (id1 == 0)
		handle_cmd1(env, argv, fd_array);
	id2 = fork ();
	if (id2 < 0)
		ft_error_after_pipe("fork\n", fd_array, 1);
	if (id2 == 0)
		handle_cmd2(env, argv, fd_array);
	close(fd_array[0]);
	close(fd_array[1]);
	waitpid(id1, &wstatus1, 0);
	waitpid(id2, &wstatus2, 0);
	int exit_status_id1;
	int exit_status_id2;
	if (WIFEXITED(wstatus1))
	{
		exit_status_id1 = WEXITSTATUS(wstatus1);
	}
	if (WIFEXITED(wstatus2))
	{
		exit_status_id2 = WEXITSTATUS(wstatus2);
	}
	//check_exit(wstatus);
	return (exit_status_id1 > exit_status_id2 ? exit_status_id1 : exit_status_id2);
}
