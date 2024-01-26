#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char **argv, char **envp)
{
	int i = 0;

	// while (envp[i])
	// {
	// 	printf("envp: %s\n", envp[i]);
	// 	i++;
	// }

	int id = fork();
	printf("id_x: %d\n", id);
	if (id == 0)
	{
		printf("child_x\n");

	}

	else
	{
		int id2 = fork();
		printf("id_y: %d\n", id2);
		if (id2 == 0)
			printf("child_y\n");
		else
		{
			// we never reach here if we do id != 0
			wait(NULL);
			printf("parent_y\n");
		}
		// if I dont have the wait here it will print parent id, "parent"
		// then the rest
		wait(NULL);
		printf("parent_x\n");
	}
	printf("hello world\n");
	return (0);
}
