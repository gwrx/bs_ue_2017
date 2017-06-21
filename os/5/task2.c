#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int fds[2];
	if(pipe(fds) == -1)
	{
		perror("pipe");
		return 1;
	}

	pid_t child = fork();
	if(child == -1)
	{
		perror("fork");
		return 1;
	}

	// parent process
	if(child)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		execlp("ls", "ls", (char*) NULL);
	}
	else // child
	{
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);

		char** args = malloc(sizeof(char*) * argc + 1);

		args[0] = "grep";

		for(int i = 1; i < argc; ++i)
		{
			args[i] = argv[i];
		}

		args[argc] = NULL;

		execvp("grep", args);
	}

	return 0;
}
