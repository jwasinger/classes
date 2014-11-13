#include "logger.h"

int fd = -1;

int log(char *str)
{
    fd = open(LOG_FILE, O_RDWR, 0);
    if(fd == -1)
        return -1;

    write(
}
