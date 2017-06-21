#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>

int main(void)
{
	sem_t* sem = sem_open("/task4", 0);
	if(sem == SEM_FAILED)
	{
		perror("sem_open");
		return 1;
	}

	int shm = shm_open("/task4", O_RDWR, 0);
	if(shm == -1)
	{
		perror("shm_open");
		return 1;
	}

	void* addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
	if(addr == MAP_FAILED)
	{
		perror("mmap");
		return 1;
	}

	if(close(shm) == -1)
	{
		perror("close");
		return 1;
	}

	int* value = addr;

	for(int i = 0; i < 100; ++i)
	{
		pid_t child = fork();
		if(child == -1)
		{
			perror("fork");
			return 1;
		}
		else if(child == 0)
		{
			for(int i = 0; i < 100; ++i)
			{
				if(sem_wait(sem) == -1)
				{
					perror("sem_wait");
					return 1;
				}

				++*value;

				if(sem_post(sem) == -1)
				{
					perror("sem_post");
					return 1;
				}
			}
			return 0;
		}
	}

	while(wait(NULL) != -1 && errno != ECHILD);

	FILE* fifo = fopen("RESULT_FIFO", "w");
	if(fifo == NULL)
	{
		perror("fopen");
		return 1;
	}

	fprintf(fifo, "%d", *value);
	fclose(fifo);

	if(sem_close(sem) == -1)
	{
		perror("sem_close");
		return 1;
	}

	if(munmap(addr, sizeof(int)) == -1)
	{
		perror("munmap");
		return 1;
	}

	return 0;
}
