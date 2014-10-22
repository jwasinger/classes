#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

#include "oscar.h"

struct ArchiveFile
{
    struct oscar_hdr hdr;
    char *file_data;
    int file_size;
};

struct Archive
{
    struct ArchiveFile *files;
    int size_files; /* capacity of 'files' array */
    int num_files; /* number of ArchiveFiles in array */
    char archive_name[32];
};

/* try and open the archive file 'file_name', create a new archive if the file doesn't exist and 'create'=1 */
int open_archive(char *file_name, struct Archive *out_archive, int create);

/* write out the archive to disk */
int write_archive(char *file_name, const struct Archive *archive);

void free_archive(struct Archive *archive);
#endif
