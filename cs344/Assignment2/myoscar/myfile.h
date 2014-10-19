#ifndef MYFILE_H
#define MYFILE_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAX_LINE_LENGH 256

/* retrieve all lines in a file specified by 'fd'. The return value is a 
   pointer to dynamically allocated 2D array of the lines */
int get_file_lines(int fd, char ***lines, int *num_lines);

/* retrieve one line from a file */
int get_file_line(int fd, int line_num, char **line);

int remove_file_line(int fd, int line_num);

#endif
