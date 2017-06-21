#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int myRandom(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		fprintf(stderr, "USAGE: %s LOG_FIFO LOG_MESSAGE...\n", argv[0]);
		fputs("At least one LOG_MESSAGE is required!\n", stderr);
		return 1;
	}

	srand(time(NULL) + argc + strlen(argv[2]));

	FILE* log = fopen(argv[1], "w");
	if(!log)
	{
		fprintf(stderr, "ERROR: can not open LOG_FIFO \"%s\": %s\n", argv[1], strerror(errno));
		return 1;
	}

	for(;;)
	{
		sleep(myRandom(2, 7));
		fprintf(log, "%s\n", argv[myRandom(2, argc - 1)]);
		fflush(log);
	}

	return 0;
}
