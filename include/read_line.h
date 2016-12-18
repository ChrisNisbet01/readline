#ifndef __IO_UTILS_READ_LINE_H__
#define __IO_UTILS_READ_LINE_H__

#include <stddef.h>

int read_line_with_timeout(char * * output_buffer, size_t * output_buffer_size, int fd, unsigned int timeout_seconds);

#endif /*  __IO_UTILS_READ_LINE_H__ */
