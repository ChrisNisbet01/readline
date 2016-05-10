#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "read_line.h"

int main (int argc, char *argv[])
{
	int fd;
	char * line = NULL;
	size_t n = 0;

	fd = open("./read_line.c", O_RDONLY);
	if (fd >= 0)
	{
		int bytes_read;

		do
		{
			bytes_read = readLine(&line, &n, fd);
			if (bytes_read > 0)
			{
				fprintf(stderr, "%s", line);
			}
		}
		while (bytes_read != -1);
	}
	close(fd);
	free(line);

	return 0;
}

