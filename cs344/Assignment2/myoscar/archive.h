/*********************
Name: Jared Wasinger
ONID Email: wasingej@onid.oregonstate.edu
Class: CS 344
Assignment #2
References:
**********************/

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
    char *file_data; //is this member ever deleted when a free(ArchiveFile) call is made?
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

/* write out the archive to disk.  only write out parts that differ from the file on disk*/
int write_archive(char *file_name, struct Archive *archive);

void free_archive(struct Archive **archive);

int archive_add_files(struct Archive *archive, char **files, int num_files);

void archive_disp_toc(const struct Archive *archive);

void archive_disp_long_toc(const struct Archive *archive);

int archive_contains_file(char *file_name, const struct Archive *archive, int **out_indices, int *out_num_indices);

int archive_extract_members(char **file_name, int num_files, const struct Archive *archive, int overwrite);

int archive_extract_members_cur_time(char **file_name, int num_files, const struct Archive *archive, int overwrite);

//delete archive files making only ONE pass through the archive file
int archive_delete_members(char **file_names, int num_files, struct Archive *archive);

int archive_cleanse(struct Archive *archive);

int archive_mark_members(char **file_names, int num_files, struct Archive *archive);

int archive_unmark_members(char **file_names, int num_files, struct Archive *archive);
#endif
