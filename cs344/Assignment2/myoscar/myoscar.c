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



int main(int argc, char **argv)
{
    if(argc == 1)
    {
        printf("*** Archive file not specified\n");
        printf("*** Exiting...\n");
		_Exit(-1);
    }
    
    CMDArgs cmd_args;
    if(proc_cmd_line_args(argc, argv, &cmd_args) == -1)
    {
        printf("Failure!\n");
        _Exit(-1);
    }
    
    
    //open_archive("test", &fd);
	return 0;
}
