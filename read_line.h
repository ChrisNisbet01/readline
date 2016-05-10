#ifndef __READ_LINE_H__
#define __READ_LINE_H__

#include <stddef.h>

int get_char_with_timeout(int const fd, unsigned int timeout_seconds, char * const character_read);
int read_line_with_timeout(char * * output_buffer, size_t * output_buffer_size, int fd, unsigned int timeout_seconds);

#endif /*  __READ_LINE_H__ */
