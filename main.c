/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbuerge <chbuerge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:40:32 by chbuerge          #+#    #+#             */
/*   Updated: 2024/01/30 18:06:52 by chbuerge         ###   ########.fr       */
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
	if (access(input[1], F_OK) == -1)
	{
		ft_printf("pipex: %s: No such file or directory\n", input[1]);
		ft_error_after_pipe("", fd_array, EXIT_FAILURE);
	}
	if (access(input[1], R_OK) == -1)
	{
		ft_printf("pipex: %s: Permission denied\n", input[1]);
		ft_error_after_pipe("", fd_array, EXIT_FAILURE);
	}
	fd = open(input[1], O_RDONLY);
	dup2(fd, STDIN_FILENO);
	dup2(fd_array[1], STDOUT_FILENO);
	close(fd_array[0]);
	if (execute(env, input, cmd, fd_array) == -1)
	{
		close(fd_array[0]);
		close(fd_array[1]);
		close(fd);
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
		ft_printf("pipex: %s: Permission denied\n", input[4]);
		ft_error_after_pipe("", fd_array, EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	dup2(fd_array[0], STDIN_FILENO);
	close(fd_array[1]);
	if (execute(env, input, cmd, fd_array) == -1)
	{
		close(fd_array[0]);
		close(fd_array[1]);
		close(fd);
		exit(127);
	}
}

int	cmd_empty(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] != 32)
			return (0);
		i++;
	}
	return (1);
}

// int	handle_exit_status(int id1, int id2)
// {
// 	int	status1 = 0;
// 	int	status2 = 0;
// 	int	exit_status = 0;
// 	int	i = 0;
// 	waitpid(id1, &status1, WUNTRACED);
// 	waitpid(id2, &status2, WUNTRACED);
// 	if(WIFEXITED(status2))
// 	{
// 		exit_status = WEXITSTATUS(status2);
// 		i = exit_status;
// 	}
// 	return (i);
// }

// open argv[1] only reading? & argv[4] allowing everything?
// open the infile argv[1] -> read only permission
// open/ create outfile argv[4] -> more permission?
int	main(int argc, char **argv, char **env)
{
	int	fd_array[2];
	int	id1;
	int	id2;

	if (argc != 5 || (cmd_empty(argv[2]) == 1) || (cmd_empty(argv[3]) == 1))
		ft_error("./pipex infile cmd1 cm2 outfile\n");
	if (!env || !*env)
		ft_error("Error: env\n");
	if (pipe(fd_array) == -1)
		ft_error("pipe\n");
	id1 = fork();
	if (id1 < 0)
		ft_error_after_pipe("fork\n", fd_array, EXIT_FAILURE);
	if (id1 == 0)
		handle_cmd1(env, argv, fd_array);
	id2 = fork ();
	if (id2 < 0)
		ft_error_after_pipe("fork\n", fd_array, EXIT_FAILURE);
	if (id2 == 0)
		handle_cmd2(env, argv, fd_array);
	close(fd_array[0]);
	close(fd_array[1]);
	waitpid(id1, NULL, 0);
	waitpid(id2, NULL, 0);
	return (0);
}
