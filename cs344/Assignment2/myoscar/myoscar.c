#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#include <unistd.h>

#include "cmd_line_args.h"
#include "myfile.h"

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

int open_archive(char *file_name, int *fd_out)
{
	int fd = -1;
	
	/* try and open the file 'file_name' */
	fd = open(file_name, O_RDWR, 0);
	if(fd == -1)
	{
		if(errno == ENOENT)
		{
			/* file doesn't exist, create it */
			printf("creating file '%s'\n", file_name);
			
			fd = open(file_name, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR);
			if(fd == -1)
			{
				error_exit();	
			}

		}
	}
	
	/* if it exists see if it is a valid archive and return -1 if not */
    char **lines = NULL;
	int num_lines;

	if(get_file_lines(fd, &lines, &num_lines) == -1)
	{
		return -1;
	}
	
    int i = 0;
	for(; i < num_lines; i++)
	{
		printf(lines[i]);
	}

	/* if it doesn't exist create the file */

	return 0;
}

int main(int argc, char **argv)
{
    if(argc == 1)
    {
        printf("*** Archive file not specified\n");
        printf("*** Exiting...\n");
		_Exit(-1);
    }
    
    CMDArgs cmd_args;
    if(proc_cmd_line_args(argc, argv, &cmd_args) != -1)
    {
        printf("success!\n");
    }
	//open_archive("test", &fd);
	return 0;
}
