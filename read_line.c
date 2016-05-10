#include "read_line.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#define MIN_ALLOC 64

int readLine(char ** output_buffer, size_t * output_buffer_size, int fd)
{
	size_t total_bytes_read;
	char * output_location;
	char const terminator = '\n';
	char const nul = '\0';

	if (output_buffer_size == NULL || output_buffer == NULL || fd < 0)
	{
		errno = EINVAL;
		return -1;
	}

	/* allocate a buffer if none was supplied */
	if ( *output_buffer == NULL)
	{
		*output_buffer_size = MIN_ALLOC;
		*output_buffer = malloc(*output_buffer_size);
		if (*output_buffer == NULL)
		{
			errno = ENOMEM;
			return -1;
		}
	}

	output_location = *output_buffer;
	total_bytes_read = 0;

	for (;;)
	{
		char ch;
		ssize_t bytes_read;

		bytes_read = read(fd, &ch, 1);
		if (bytes_read == -1)
		{
			if (errno == EINTR)
				continue;
			if (total_bytes_read > 0)	/* read something prior to this error */
				break;
			return -1;
		}

		/* No error. Was anything read? */
		if (bytes_read == 0)      /* EOF */
		{
			if (total_bytes_read > 0)
				break;
			return -1;
		}
		/* read a chaaracter */

		/* Ensure we have space to write it to the output buffer */
		if (total_bytes_read >= *output_buffer_size - 1)	/* must be space for the new char and the NUL terminator */
		{
			*output_buffer_size *= 2;
			*output_buffer = realloc(*output_buffer, *output_buffer_size);
			if (*output_buffer == NULL)
			{
				errno = ENOMEM;
				return -1;
			}
			output_location = *output_buffer + total_bytes_read;
		}

		/* add it to the output buffer */
		*output_location++ = ch;
		total_bytes_read++;

		if (ch == terminator)
			break;
	}

	*output_location = nul;

	return total_bytes_read;
}


