#include "read_line.h"
#include "get_char_with_timeout.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MIN_ALLOC 64

int read_line_with_timeout(char * * output_buffer, size_t * output_buffer_size, int fd, unsigned int timeout_seconds)
{
    size_t total_bytes_read;
    char const terminator = '\n';
    char const nul = '\0';
    int readline_result;

    if (output_buffer_size == NULL || output_buffer == NULL || fd < 0)
    {
        errno = EINVAL;
        return -1;
    }

    /* allocate a buffer if none was supplied */
    if (*output_buffer == NULL)
    {
        *output_buffer_size = MIN_ALLOC;
        *output_buffer = malloc(*output_buffer_size);
        if (*output_buffer == NULL)
        {
            errno = ENOMEM;
            return -1;
        }
    }

    total_bytes_read = 0;

    for (;;)
    {
        char ch;
        int get_char_result;

        get_char_result = get_char_with_timeout(fd, timeout_seconds, &ch);
        if (get_char_result == -1)
        {
            readline_result = -1;
            goto done;
        }

        /* No error. Was anything read? */
        if (get_char_result == 0)      /* EOF */
        {
            /* The final line in a file doesn't need to be terminated with 
             * '\n'. 
             */
            if (total_bytes_read > 0)
            {
                break;
            }
            readline_result = -1;
            goto done;
        }
        /* read a character */

        /* Ensure we have space to write it to the output buffer */
        if (total_bytes_read >= *output_buffer_size - 1)    /* must be space for the new char and the NUL terminator */
        {
            *output_buffer_size *= 2;
            *output_buffer = realloc(*output_buffer, *output_buffer_size);
            if (*output_buffer == NULL)
            {
                errno = ENOMEM;
                readline_result = -1;
                goto done;
            }
        }

        /* add it to the output buffer */
        (*output_buffer)[total_bytes_read] = ch;
        total_bytes_read++;

        if (ch == terminator)
        {
            break;
        }
    }

    (*output_buffer)[total_bytes_read] = nul;
    readline_result = total_bytes_read;

done:
    return readline_result;
}

