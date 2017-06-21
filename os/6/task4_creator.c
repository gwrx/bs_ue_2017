#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

int main(void)
{
	sem_t* sem = sem_open("/task4", O_RDWR | O_CREAT | O_EXCL, 0600, 1);
	if(sem == SEM_FAILED)
	{
		perror("sem_open");
		return 1;
	}

	if(sem_close(sem) == -1)
	{
		perror("sem_close");
		return 1;
	}

	int shm = shm_open("/task4", O_RDWR | O_CREAT | O_EXCL, 0600);
	if(shm == -1)
	{
		perror("shm_open");
		return 1;
	}

	if(ftruncate(shm, sizeof(int)) == -1)
	{
		perror("ftruncate");
		return 1;
	}

	// memory is automatically initialized to 0

	if(close(shm) == -1)
	{
		perror("close");
		return 1;
	}

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

	if(sem_unlink("/task4") == -1)
	{
		perror("sem_unlink");
		return 1;
	}

	if(shm_unlink("/task4") == -1)
	{
		perror("shm_unlink");
		return 1;
	}

	return 0;
}
