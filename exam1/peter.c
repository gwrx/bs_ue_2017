#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>



typedef struct smem_content {
	int student_id;
	pid_t student_process_id;
	char result[30];
} smem_content;


int myRandom(int min, int max) {
	srand(time(NULL));
	return rand() % (max - min + 1) + min;
}



int main(void) {

	int nstudents = 30;
	int studentresults[nstudents];
	int max_exercise = nstudents * 5;
	int sentresults = 0;

	FILE* fifo = fopen("REQUEST_FIFO", "r");

	if (!fifo) {
		printf("Peter: Error opening Fifo\n");
		return EXIT_FAILURE;
	}


	while(true) {


		int result;
		fscanf(fifo, "%d", &result);
		fclose(fifo);

		int mem = shmget(1234, sizeof(smem_content), IPC_CREAT | IPC_EXCL | 0600);
		if (mem == -1) {
			printf("Peter: error shmget\n");
			return EXIT_FAILURE;
		}

		void* addr = shmat(mem, NULL, 0);
		if (addr == (void*)-1) {
			printf("Peter: error shmat\n");
			return EXIT_FAILURE;
		}

		smem_content* content = addr;

		if (result == content->student_id) {
			studentresults[content->student_id]++;
			sentresults++;

			if (studentresults[content->student_id] == 5) {
				printf("Grade for Student %d: %d\n", content->student_id, myRandom(1, 5));
			}

			if (max_exercise == sentresults) {
				printf("That's all folks...\n");
				return EXIT_SUCCESS;
			}

		}
	}



	return EXIT_SUCCESS;
}
