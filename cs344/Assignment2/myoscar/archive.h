#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

#include "oscar.h"

typedef struct Archive;

/* try and open the archive file 'file_name', create a new archive if the file doesn't exist */
int open_archive(char *file_name, Archive *out_archive);

/* write out the archive to disk */
int write_archive(char *file_name, Archive archive);

#endif
