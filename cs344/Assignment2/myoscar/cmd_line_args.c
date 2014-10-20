#include "cmd_line_args.h"

int proc_cmd_line_args(int argc, char **argv, CMDArgs *out_cmd_args)
{
    if(!out_cmd_args || !argv)
        return -1;
    
    memset(out_cmd_args, 0, sizeof(CMDArgs));
    
    int num_files = 0;
    int files_write_pos = 0;
    int actions = 0;
    int num_actions = 0;
    int i = 0;
    for(; i < argc; i++)
    {
        if(strcmp(argv[i], "-a") == 0)
        {
            //note: this option always takes precendence over -A
            actions |= ACTION_ADD_MEMBERS;
            num_actions++;
        }
        if(strcmp(argv[i], "-A") == 0)
        {
            actions |= ACTION_ADD_ALL;
            num_actions++;
        }
        if(strcmp(argv[i], "-C") == 0)
        {
            actions |= ACTION_CLEANSE;
            num_actions++;
        }
        if(strcmp(argv[i], "-d") == 0)
        {
            actions |= ACTION_DELETE;
            num_actions++;
        }
        if(strcmp(argv[i], "-e") == 0)
        {
            actions |= ACTION_EXTRACT;
            num_actions++;
        }
        if(strcmp(argv[i], "-E") == 0)
        {
            actions |= ACTION_EXTRACT_CURR_TIME;
            num_actions++;
        }
        if(strcmp(argv[i], "-h") == 0)
        {
            actions |= ACTION_HELP;
            num_actions++;
        }
        if(strcmp(argv[i], "-m") == 0)
        {
            actions |= ACTION_MARK;
            num_actions++;
        }
        if(strcmp(argv[i], "-o") == 0)
        {
            actions |= ACTION_OVERWRITE;
            num_actions++;
        }
        if(strcmp(argv[i], "-t") == 0)
        {
            actions |= ACTION_TOC;
            num_actions++;
        }
        if(strcmp(argv[i], "-T") == 0)
        {
            actions |= ACTION_LONG_TOC;
            num_actions++;
        }
        if(strcmp(argv[i], "-u") == 0)
        {
            actions |= ACTION_UNMARK;
            num_actions++;
        }
        if(strcmp(argv[i], "-v") == 0)
        {
            actions |= ACTION_VERBOSE;
            num_actions++;
        }
        if(strcmp(argv[i], "-V") == 0)
        {
            actions |= ACTION_VERSION;
            num_actions++;
        }
        else if (!out_cmd_args->arc_file)
        {
            strcpy(out_cmd_args->arc_file, argv[i]);
            continue;
        }
        else
        {
            //the command line arg can only be an archive file member
            if(!out_cmd_args->files)
            {
                num_files = argc - (num_actions+1);

                out_cmd_args->files = malloc(sizeof(char*) * num_files);
                out_cmd_args->files[files_write_pos] = malloc(sizeof(char) * strlen(argv[i]));
            }
        }
    }
  
    out_cmd_args->num_files = num_files;
    return 0;
}
