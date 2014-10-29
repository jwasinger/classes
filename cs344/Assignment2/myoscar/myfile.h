#ifndef MYFILE_H
#define MYFILE_H

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>

int load_pwd_files(char ***files_data_out, int **sizes_out, char ***file_names_out, int *num_files);
int load_pwd_reg_files(char ***files_data_out, int **sizes_out, char ***file_names_out, int *num_files);

int get_pwd_reg_files(char ***file_names, int *num_files, char *arc_file_name, char *prog_name);

/* internal functions in myfile.c

int __get_pwd_reg_files(char ***file_names, int *num_files)

int __get_pwd_files(char ***file_names, int *num_files)

int __load_file(char *file_name, char **file_data, int *size)

void __free_arrays(char **files_data, char **file_names, int *sizes, int num)
*/
#endif
