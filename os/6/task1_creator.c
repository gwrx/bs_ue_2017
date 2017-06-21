#include <stdio.h>
#include <sys/shm.h>

int main(void)
{
	int mem = shmget(1234, sizeof(int), IPC_CREAT | IPC_EXCL | 0600);
	if(mem == -1)
	{
		perror("shmget");
		return 1;
	}

	// memory is automatically initialized to 0

	FILE* fifo = fopen("RESULT_FIFO", "r");
	if(fifo == NULL)
	{
		perror("fopen");
		return 1;
	}

	int result = 0;
	fscanf(fifo, "%d", &result);
	printf("%d\n", result);
	fclose(fifo);

	if(shmctl(mem, IPC_RMID, NULL) == -1)
	{
		perror("shmctl");
		return 1;
	}

	return 0;
}
