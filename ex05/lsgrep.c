#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char * argv[]) {

	printf("%d", argc);

	if (argc <= 1) {
		printf("usage: %s <keyword>", argv[0]);
	}

	int p[2];
	int pid;
	int r;
	char *ls[] = {"ls", NULL};
	char *grep[] = {"grep", argv[1], NULL};

	pipe(p);

	pid = fork();
	if (pid  != 0) {
		
		// Parent: Output is to child via pipe[1]
		
		// Change stdout to pipe[1]
		dup2(p[1], 1);
		close(p[0]);

		r = execvp("ls", ls);
	} else {
		// Child: Input is from pipe[0] and output is via stdout.
		dup2(p[0], 0);
		close(p[1]);

		r = execvp("grep", grep);
		close(p[0]);
	}

	return r;
}
