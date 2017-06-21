#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>


typedef struct smem_content {
	int student_id;
	pid_t student_process_id;
	char result[30];
} smem_content;


int myRandom(int min, int max) {
	srand(time(NULL));
	return rand() % (max - min + 1) + min;
}



int main(int argc, char* argv[]) {

	if (argc != 2) {
		printf("Student: error arguments\n");
		return EXIT_FAILURE;
	}

	FILE* fifo = fopen("REQUEST_FIFO", "w");
	if (fifo == NULL) {
		printf("Student: error opening fifo\n");
		return EXIT_FAILURE;
	}

	int student_id = atoi(argv[1]);

	for (int i = 0; i < 5; i++) {	

		smem_content content;

		int mem = shmget(1234, sizeof(content), 0);

		void* addr = shmat(mem, NULL, 0);
		if (addr == (void*) -1) {
			printf("Student: error shmat\n");
			return EXIT_FAILURE;
		}

		if (mem == -1) {
			printf("Student: error shmget\n");
			return EXIT_FAILURE;
		}

		sleep(myRandom(2, 5));
		content.student_id = student_id;
		content.student_process_id = getpid();
		content.result[0] = 1;
		fprintf(fifo, "%d", getpid());
	}


	return EXIT_SUCCESS;
}
