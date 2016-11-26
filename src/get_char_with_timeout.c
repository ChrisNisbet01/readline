#include "get_char_with_timeout.h"

#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <stddef.h>

#if !defined(TEMP_FAILURE_RETRY)

/* Evaluate EXPRESSION, and repeat as long as it returns -1 with `errno'
   set to EINTR.  */

#define TEMP_FAILURE_RETRY(expression) \
  (__extension__							      \
    ({ long int __result;						      \
       do __result = (long int) (expression);				      \
       while (__result == -1L && errno == EINTR);			      \
       __result; }))
#endif

int get_char_with_timeout(int const fd, unsigned int const timeout_seconds, char * const character_read)
{
    fd_set fdset;
    int select_result;
    int get_char_result;
    struct timeval timeout;
    struct timeval * timeout_to_use;

    if (timeout_seconds > 0)
    {
        timeout.tv_sec = timeout_seconds;
        timeout.tv_usec = 0;
        timeout_to_use = &timeout;
    }
    else
    {
        timeout_to_use = NULL;
    }

	FD_ZERO(&fdset);
    FD_SET(fd, &fdset);

    select_result = TEMP_FAILURE_RETRY(select(fd + 1, &fdset, NULL, NULL, timeout_to_use));

    if (select_result == -1)
    {
        get_char_result = -1;
        goto done;
    }

	if (select_result == 0)
    {
        errno = ETIMEDOUT;
        get_char_result = -1;
        goto done;
    }

	if (!FD_ISSET(fd, &fdset)) /* Shouldn't happen. */
    {
        get_char_result = -1;
        goto done;
    }

    get_char_result = TEMP_FAILURE_RETRY(read(fd, character_read, sizeof *character_read));

done:
    return get_char_result;
}


