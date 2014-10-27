#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#include <unistd.h>

#include "cmd_line_args.h"
#include "myfile.h"
#include "archive.h"
#include "toc.h"

int error_exit()
{
	printf("ERROR: %s\n", strerror(errno));
	_Exit(-1);
}


int print_verbose(char *str)
{
	/* TODO: only print str if verbose mode is enabled */
	char *print_str = strcat(str, "\n");
	printf(print_str);
	return 0;
}

int add_files(struct Archive *archive, char **files, int num_files)
{
    
}

int main(int argc, char **argv)
{
    struct Archive *archive = NULL;
    struct CMDArgs *cmd_args = malloc(sizeof(struct CMDArgs));
    int res = 0;
    int i = 0;

    if(argc == 1)
    {
        printf("*** Archive file not specified\n");
        printf("*** Exiting...\n");
		_Exit(-1);
    }

    if(proc_cmd_line_args(argc, argv, cmd_args) == -1)
    {
        printf("Failure!\n");
        _Exit(-1);
    }

    //if(open_archive("test", archive) == -1)
    //{
    //    printf("failure\n");
    //    _Exit(-1);
    //}
    
    //if there is no archive file supplied exit

    
    res = open_archive(cmd_args->arc_file, &archive, 1);
    if(res == -1)
    {   
        _Exit(-1);
    }

    if (cmd_args->actions & ACTION_ADD_MEMBERS)
    {
        if(cmd_args->num_files == 0)
        {
            printf("no files specified to archive... exiting...\n");
            _Exit(-1);
        }     
        
        //add the values
        res = archive_add_files(archive, cmd_args->files, cmd_args->num_files); 
        if(res==-1)
            return -1;

        return 0;
    }
    else if (cmd_args->actions & ACTION_ADD_ALL)
    {
        if(cmd_args->num_files == 0)
        {
            printf("no files specified to archive... exiting...\n");
            _Exit(-1);
        }     

        char **file_names;
        int num_files;
        res = get_pwd_reg_files(&file_names, &num_files);
        if(res == -1)
        {
            return -1; 
        }
    
        res = archive_add_files(archive, file_names, num_files);
        if(res == -1)
        {
            free(file_names);
            return -1;
        }

        free(file_names);
        return 0;
    }
    else if (cmd_args->actions & ACTION_TOC)
    {
        disp_archive_toc(archive);
    }
    else if (cmd_args->actions & ACTION_LONG_TOC)
    {
        disp_archive_long_toc(archive);         
    }
    else if (cmd_args->actions & ACTION_EXTRACT)
    {
        res = archive_extract_member(cmd_args->files[0], archive);
        if(res == -1)
        {
            return -1;    
        }
    }
    else if (cmd_args->actions & ACTION_EXTRACT_CURR_TIME)
    {
        
    }
    free_archive(&archive);
    free_CMDArgs(&cmd_args);
	return 0;
}
