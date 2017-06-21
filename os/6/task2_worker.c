#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

int main(void)
{
	int sem = semget(1234, 0, 0);
	if(sem == -1)
	{
		perror("semget");
		return 1;
	}

	int mem = shmget(1234, sizeof(int), 0);
	if(mem == -1)
	{
		perror("shmget");
		return 1;
	}

	void* addr = shmat(mem, NULL, 0);
	if(addr == (void*)-1)
	{
		perror("shmat");
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
			struct sembuf sop;
			sop.sem_num = 0;
			sop.sem_flg = 0;

			for(int i = 0; i < 100; ++i)
			{
				sop.sem_op = -1;
				if(semop(sem, &sop, 1) == -1)
				{
					perror("semop");
					return 1;
				}

				++*value;

				sop.sem_op = +1;
				if(semop(sem, &sop, 1) == -1)
				{
					perror("semop");
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

	shmdt(addr);

	return 0;
}
