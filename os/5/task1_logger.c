#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/select.h>
#include <limits.h>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s LOG_FIFO...\n", argv[0]);
		fputs("At least one LOG_FIFO is required!\n", stderr);
		return 1;
	}

	int* fds = malloc(sizeof(int) * (argc - 1));

	for(int i = 0; i < argc - 1; ++i)
	{
		fds[i] = open(argv[i + 1], O_RDONLY | O_NONBLOCK);

		if(fds[i] == -1)
		{
			fprintf(stderr, "ERROR: can not open LOG_FIFO \"%s\": %s\n", argv[i + 1], strerror(errno));
		}
	}

	char buf[PIPE_BUF];

	int nfds;
	fd_set fdset;

	for(;;)
	{
		FD_ZERO(&fdset);
		for(int i = 0; i < argc - 1; ++i)
		{
			if(fds[i] != -1)
			{
				FD_SET(fds[i], &fdset);

				if(fds[i] > nfds)
				{
					nfds = fds[i];
				}
			}
		}
		++nfds;

		int ret = select(nfds, &fdset, NULL, NULL, NULL);

		if(ret < 0)
		{
			if(errno == EINTR || errno == EAGAIN)
			{
				continue;
			}
			else
			{
				perror("select");
				return 1;
			}
		}

		if(ret > 0)
		{
			for(int i = 0; i < argc - 1; ++i)
			{
				if(fds[i] != -1 && FD_ISSET(fds[i], &fdset))
				{
					int count = read(fds[i], buf, sizeof(buf));
					if(count > 0)
					{
						printf("[%s] ", argv[i + 1]);
						fflush(stdout);
						write(STDOUT_FILENO, buf, count);
					}
					/*else
					{
						close(fds[i]);
						fds[i] = -1;
					}*/
				}
			}
		}
	}

	return 0;
}
