#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define WEBFIFO       "tmp/webfifo"
#define DBFIFO        "tmp/dbfifo"
#define MWFIFO        "tmp/mwfifo"


int main(void) {

	int fd;
	char * webfifo = WEBFIFO;
	char * dbfifo = DBFIFO;
	char * mwfifo = MWFIFO;

	int buffer = 80;

	int r;

	/* create the FIFO (named pipe) */
	mkfifo(webfifo, 0666);
	mkfifo(dbfifo, 0666);
	mkfifo(mwfifo, 0666);

	for (int i = 0; i < 20; i++) {

		srand(time(NULL));
		r = (rand() % 6) + 2;

		printf("entering loop, r = %d\n", r);

		sleep(r);

		int choose = i % 3;

		switch(choose) {
			case 0: {
				/* write "Hi" to the FIFO */
				fd = open(webfifo, O_WRONLY);
				write(fd, "Hi web", buffer);
				close(fd);
				printf("sent: Hi web %d\n", i);
				break;
				}
			case 1: {
				fd = open(dbfifo, O_WRONLY);
				write(fd, "Hi db", buffer);
				close(fd);
				printf("sent: Hi db %d\n", i);
				break;
				}
			case 2: {
				fd = open(mwfifo, O_WRONLY);
				write(fd, "Hi mw", buffer);
				close(fd);
				printf("sent: Hi mw %d\n", i);
				break;
				}
		}
	}


	/* remove the FIFO */
	unlink(webfifo);
	unlink(dbfifo);
	unlink(mwfifo);


	return EXIT_SUCCESS;
}

