#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utime.h>

#include "oscar.h"
#include "myfile.h"
#include "cmd_line_args.h"

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
int open_archive(char *file_name, struct Archive **out_archive, int create);

/* write out the archive to disk */
int write_archive(char *file_name, struct Archive *archive);

void free_archive(struct Archive **archive);

int archive_add_files(struct Archive *archive, char **files, int num_files);

int archive_del_files(struct Archive *archive, char *file_names, int num_files); 

int archive_add_reg_files(struct Archive *archive); //add all regular files in the pwd

void archive_disp_toc(const struct Archive *archive);

void archive_disp_long_toc(const struct Archive *archive);

int archive_contains_file(char *file_name, const struct Archive *archive, int *out_index);

int archive_extract_member(char *file_name, const struct Archive *archive);

int archive_extract_member_cur_time(char *file_name, const struct Archive *archive);
#endif
