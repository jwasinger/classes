/*********************
Name: Jared Wasinger
ONID Email: wasingej@onid.oregonstate.edu
Class: CS 344
Assignment #2
References: 
**********************/

#ifndef CMD_LINE_ARGS_H
#define CMD_LINE_ARGS_H

#include <string.h>
#include <stdio.h>
#include "oscar.h"

#define ACTION_ADD_MEMBERS          1
#define ACTION_ADD_ALL              2    
#define ACTION_CLEANSE              4
#define ACTION_DELETE               8
#define ACTION_EXTRACT              16
#define ACTION_EXTRACT_CURR_TIME    32
#define ACTION_HELP                 64
#define ACTION_MARK                 128
#define ACTION_OVERWRITE            256
#define ACTION_TOC                  512
#define ACTION_LONG_TOC             1024
#define ACTION_UNMARK               2048
#define ACTION_VERBOSE              4096
#define ACTION_VERSION              8192

#define ERROR_INVALID_ARC           1
#define ERROR_NO_ACTIONS            2

struct CMDArgs
{
    char arc_file[OSCAR_MAX_FILE_NAME_LEN];
    int has_arc_file;
    
    char **files;
    int num_files;
    int actions;
};

int proc_cmd_line_args(int argc, char **argv, struct CMDArgs *out_cmd_args);

void free_CMDArgs(struct CMDArgs **cmd_args);
#endif
