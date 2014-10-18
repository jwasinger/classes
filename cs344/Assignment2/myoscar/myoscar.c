#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#define ACTION_ADD_MEMBERS          0
#define ACTION_ADD_ALL              1    
#define ACTION_CLEANSE              2
#define ACTION_DELETE               3
#define ACTION_EXTRACT              4
#define ACTION_EXTRACT_CURR_TIME    5
#define ACTION_HELP                 6 
#define ACTION_MARK                 7
#define ACTION_OVERWRITE            8
#define ACTION_TOC                  9
#define ACTION_LONG_TOC             10
#define ACTION_UNMARK               11
#define ACTION_VERBOSE              12
#define ACTION_VERSION              13

int error_exit()
{
	printf("ERROR: %s\n", strerror(errno));
	_Exit(-1);
}

int get_action(char *c_arg)
{
    if(strcmp(c_arg, "-a") == 0)
    {
        return ACTION_ADD_MEMBERS;    
    }
    if(strcmp(c_arg, "-A") == 0)
    {
        return ACTION_ADD_ALL; 
    }
    if(strcmp(c_arg, "-C") == 0)
    {
        return ACTION_CLEANSE;
    }
    if(strcmp(c_arg, "-d") == 0)
    {
        return ACTION_DELETE;
    }
    if(strcmp(c_arg, "-e") == 0)
    {
        return ACTION_EXTRACT;
    }
    if(strcmp(c_arg, "-E") == 0)
    {
        return ACTION_EXTRACT_CURR_TIME;
    }
    if(strcmp(c_arg, "-h") == 0)
    {
        return ACTION_HELP;     
    }
    if(strcmp(c_arg, "-m") == 0)
    {
        return ACTION_MARK;
    }
    if(strcmp(c_arg, "-o") == 0)
    {
        return ACTION_OVERWRITE;
    }
    if(strcmp(c_arg, "-t") == 0)
    {
        return ACTION_TOC;
    }
    if(strcmp(c_arg, "-T") == 0)
    {
        return ACTION_LONG_TOC; 
    }
    if(strcmp(c_arg, "-u") == 0)
    {
        return ACTION_UNMARK;
    }
    if(strcmp(c_arg, "-v") == 0)
    {
        return ACTION_VERBOSE;
    }
    if(strcmp(c_arg, "-V") == 0)
    {
        return ACTION_VERSION;
    }

    return -1;
}

int print_verbose(char *str)
{
	/* TODO: only print str if verbose mode is enabled */
	printf(strcat(str, "\n"));
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
    
    /* see what action(s) to be performed */
    char *action = argv[1];
    
    printf("%d\n",get_action(argv[1]));

	int fd;
	open_archive("asdf", &fd);
	return 0;
}
