#include <stdlib.h>
#include <stdio.h>

#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h> /* System V ipc */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO "tmp/RESULT_FIFO"
#define SHMSZ 27


int main(void) {


	key_t key = 42;

	if (shmget(key, SHMSZ, 0666) == -1) {
		exit(EXIT_FAILURE);
	}
	if (shmat(key, NULL, 0) == (void *)-1) {
		exit(EXIT_FAILURE);
	}


	char * fifo = FIFO;
	mkfifo(fifo, 0666);
	int fd = open(fifo, O_WRONLY); /* blocks anyway, no need for wait() */
	printf("%s", fifo);
	close(fd);




	return EXIT_SUCCESS;
}
