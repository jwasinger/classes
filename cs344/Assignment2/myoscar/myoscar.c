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
    
    if (cmd_args->actions & ACTION_VERBOSE)
    {
        printf("Hello world! Printing in verbose mode!\n");
    }

    if (cmd_args->actions & ACTION_VERSION)
    {
        printf("MYOSCAR Archiving Utility (1.0).  Author: Jared Wasinger\n");
        _Exit(0);
    }
    else if(cmd_args->actions & ACTION_HELP)
    {
        printf("\n\
                Options---------------------------------\n\
                -a [archive file name] [file 1] [file 2] ...\n\
                    adds file(s) to the archive. Creates a new archive file if one is not found\n\
                -A [archive file name]\n\
                    adds all regular files in the cwd to the archive file with the exception of the archive\n\
                    file itself and the myoscar executable.\n\
                -d [archive file name] [file 1] [file 2] ...\n\
                    deletes specified file(s) out of the archive.\n\
                -m [archive file name] [file 1] [file 2] ...\n\
                    marks specified file(s) as being deleted in the archive.\n\
                -u [archive file name] [file 1] [file 2] ...\n\
                    unmarks specified file(s) as being deleted in the archive.\n\
                -C [archive file name] \n\
                    deletes all marked files from the archive\n\
                -e [archive file name] [file 1] [file 2] ...\n\
                    extract specified file(s) to the cwd.  If the -o option is specified, then the file(s) will\n\
                    overwrite existing file(s) with the same name. \n\
                -t [archive file name]\n\
                    list a short table of contents\n\
                -T [archive file name]\n\
                    list a long table of contents\n\
                -v\n\
                    verbose processing\n\
                -V\n\
                    print version information\n\
        \n");
        _Exit(0);
    }
    
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
        
        res = write_archive(archive->archive_name, archive);
        if(res == -1)
        {
            return -1;
        }

        return 0;
    }
    else if (cmd_args->actions & ACTION_ADD_ALL)
    {
        char **file_names;
        int num_files;
        res = get_pwd_reg_files(&file_names, &num_files, cmd_args->arc_file, "myoscar");
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
        
        res = write_archive(archive->archive_name, archive);
        if(res == -1)
        {
            return -1;
        }

        return 0;
    }
    else if (cmd_args->actions & ACTION_TOC)
    {
        disp_archive_toc(archive);
    }
    else if (cmd_args->actions & ACTION_LONG_TOC)
    {
        if(!cmd_args->has_arc_file)
        {
            printf("no archive file specified... exiting...\n");
            return -1;
        }

        disp_archive_long_toc(archive);         
    }
    else if (cmd_args->actions & ACTION_EXTRACT)
    { 
        if(cmd_args->num_files == 0)
        {
            printf("no files specified to extract... exiting...\n");
            return -1;
        }
        else if(!cmd_args->has_arc_file)
        {
            printf("no archive file specified... exiting...\n");
            return -1;
        }
    
        if(cmd_args->actions & ACTION_OVERWRITE)
        {
            res = archive_extract_member(cmd_args->files[0], archive, 1);
        }
        else
        {
            res = archive_extract_member(cmd_args->files[0], archive, 0);
        }
        
        if(res == -1)
        {
            return -1;    
        }
    }
    else if (cmd_args->actions & ACTION_EXTRACT_CURR_TIME)
    {
        if(cmd_args->num_files == 0)
        {
            printf("no files specified to extract... exiting...\n");
            return -1;
        }
        else if(!cmd_args->has_arc_file)
        {
            printf("no archive file specified... exiting...\n");
            return -1;
        }
        
        if(cmd_args->actions & ACTION_OVERWRITE)
        {
            res = archive_extract_member_cur_time(cmd_args->files[0], archive, 1);
        }
        else
        {
            res = archive_extract_member_cur_time(cmd_args->files[0], archive, 0);
        }
        
        if(res == -1)
        {
            return -1;
        }
    }
    else if(cmd_args->actions & ACTION_DELETE)
    {
        if(cmd_args->num_files == 0)
        {
            printf("no files specified to extract... exiting...\n");
            return -1;
        }
        else if(!cmd_args->has_arc_file)
        {
            printf("no archive file specified... exiting...\n");
            return -1;
        }

        res = archive_delete_members(cmd_args->files, cmd_args->num_files, archive);
        if(res == -1)
        {
            return -1;    
        }
        
        res = write_archive(archive->archive_name, archive);
        if(res == -1)
        {
            return -1;
        }
    }
    else if(cmd_args->actions & ACTION_MARK)
    {
        res = archive_mark_members(cmd_args->files, cmd_args->num_files, archive);
        if(res == -1)
            return -1;

        res = write_archive(archive->archive_name, archive);
        if(res == -1)
            return -1;
    }
    else if(cmd_args->actions & ACTION_UNMARK)
    {
        res = archive_unmark_members(cmd_args->files, cmd_args->num_files, archive);
        if(res == -1)
            return -1;
        
        res = write_archive(archive->archive_name, archive);
        if(res == -1)
            return -1;
    }
    else if(cmd_args->actions & ACTION_CLEANSE)
    {
        res = archive_cleanse(archive);

        res = write_archive(archive->archive_name, archive);
        if(res == -1)
            return -1;
    }

    //free_archive(&archive);
    free_CMDArgs(&cmd_args);
	return 0;
}
