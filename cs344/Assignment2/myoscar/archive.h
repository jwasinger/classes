#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

#include "oscar.h"

struct Archive;

/* try and open the archive file 'file_name', create a new archive if the file doesn't exist */
int open_archive(char *file_name, struct Archive *out_archive);

/* write out the archive to disk */
int write_archive(char *file_name, struct Archive *archive);

int read_archive(int fd, struct Archive **archive);
#endif
