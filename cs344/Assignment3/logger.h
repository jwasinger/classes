#ifndef LOGGER_H
#define LOGGER_H

#define LOG_FILE "./err.log"

#include <stdio.h>
#include <fcntl.h>

void log(char *str);

#endif
