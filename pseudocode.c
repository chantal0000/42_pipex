// 1. main
	// check the number of command line arguments
	// create a pipe to communicate between child and parent
	// fork a child process
// 2. child function
	// open argv[1] and retrieve its fd
	// redirect the stdin to the open file dup2
	// redirect stdout to the write end of the pipe dup2
	// close the unused read end of the pipe
	// call execve to execute the command
