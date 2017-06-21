#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/select.h>

#include <linux/stat.h>

#define WEBFIFO     "tmp/webfifo"
#define DBFIFO      "tmp/dbfifo"
#define MWFIFO      "tmp/mwfifo"

int main(void) {

	char readbuf[80];

	fd_set rfds;

	int watchedfds[3];

	watchedfds[0] = open(WEBFIFO, O_RDONLY);
	watchedfds[1] = open(DBFIFO, O_RDONLY);
	watchedfds[2] = open(MWFIFO, O_RDONLY);

	int numofwatchedfds = sizeof(watchedfds) / sizeof(int);


	while (true) {

		FD_ZERO(&rfds);

		int maxfd = 0;
		int fifoselector = 0;

		for (fifoselector = 0; fifoselector < numofwatchedfds; fifoselector++) {
			FD_SET(watchedfds[fifoselector], &rfds);
			maxfd = (maxfd > watchedfds[fifoselector]) ? maxfd : watchedfds[fifoselector];
		}

		/* int retval = select(maxfd + 1, &rfds, NULL, NULL, NULL); */
		int retval = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);

		if (retval == -1) {
			//printf("some error while select()\n");
		} else if (retval == 0) {
			printf("the other side closed the fifo\n");
		} else {
			for (fifoselector = 0; fifoselector < numofwatchedfds; fifoselector++) {
				if (FD_ISSET(watchedfds[fifoselector], &rfds) && fifoselector == 0) {
					FILE *fp = fopen(WEBFIFO, "r");
					fgets(readbuf, 80, fp);
					printf("%s\n", readbuf);
					fclose(fp);
				}
				if (FD_ISSET(watchedfds[fifoselector], &rfds) && fifoselector == 1) {
					FILE *fp = fopen(DBFIFO, "r");
					fgets(readbuf, 80, fp);
					printf("%s\n", readbuf);
					fclose(fp);
				}
				if (FD_ISSET(watchedfds[fifoselector], &rfds) && fifoselector == 2) {
					FILE *fp = fopen(MWFIFO, "r");
					fgets(readbuf, 80, fp);
					printf("%s\n", readbuf);
					fclose(fp);
				}
			}
		}
	}

	return EXIT_SUCCESS;
}
