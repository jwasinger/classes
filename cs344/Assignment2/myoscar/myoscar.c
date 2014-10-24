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

    if(cmd_args->num_files == 0)
    {
        printf("no files specified... exiting...\n");
        _Exit(-1);
    }
    res = open_archive(cmd_args->arc_file, &archive, 1);
    if(res == -1)
    {   
        _Exit(-1);
    }

    if (cmd_args->actions & ACTION_ADD_MEMBERS)
    {
        //create an array containing all the files that need to be added
        char **file_array = malloc(sizeof(char *)*cmd_args->num_files - 1);
        for(i = 1; i < cmd_args->num_files; i++)
        {
            file_array[i] = cmd_args->files[i];
        }
         
        //add the values
        res = archive_add_files(archive, file_array, cmd_args->num_files -1); 
        free(file_array);
        if(res==-1)
            return -1;

        return 0;
    }
    else if (cmd_args->actions & ACTION_ADD_ALL)
    {

        return 0;
    }
    free_archive(&archive);
    free_CMDArgs(&cmd_args);
	return 0;
}
