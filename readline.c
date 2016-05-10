#include "read_line.h"

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct termios saved_attributes;

void
reset_input_mode (void)
{
  tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void
set_input_mode (void)
{
  struct termios tattr;
  char *name;

  /* Make sure stdin is a terminal. */
  if (!isatty (STDIN_FILENO))
    {
      fprintf (stderr, "Not a terminal.\n");
      exit (EXIT_FAILURE);
    }

  /* Save the terminal attributes so we can restore them later. */
  tcgetattr (STDIN_FILENO, &saved_attributes);
  atexit (reset_input_mode);

  /* Set the funny terminal modes. */
  tcgetattr (STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
  tattr.c_cc[VMIN] = 1;
  tattr.c_cc[VTIME] = 0;
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}


int main (int argc, char *argv[])
{
	int fd;
	char * line = NULL;
	size_t n = 0;

    set_input_mode();

	fd = open("./read_line.c", O_RDONLY);
	if (fd >= 0)
	{
		int bytes_read;

		do
		{
			bytes_read = read_line_with_timeout(&line, &n, fd, 5);
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

