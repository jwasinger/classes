#include "toc.h"
/*tlpi page 281 */

char *__get_mode_str(mode_t mode)
{
    char *result = malloc(sizeof(char) * 10);
    
    //populate first bit with file type
    if(S_ISREG(mode))
    {
        result[0] = '-';
    }
    else if(S_ISDIR(mode))
    {
        result[0] = 'd';
    }
    else if(S_ISCHR(mode))
    {
        result[0] = 'c';
    }
    else if(S_ISBLK(mode))
    {
        result[0] = 'b';
    }
    else if(S_ISFIFO(mode))
    {
        result[0] = 'p';
    }
    else if(S_ISLNK(mode))
    {
        result[0] = 'l';
    }
    else if(S_ISSOCK(mode))
    {
        result[0] = 's';
    }

    //determine which permission bits are set on user/group/other fields 
}

void disp_archive_toc(struct Archive *archive)
{
    
}

void disp_archive_long_toc(struct Archive *archive)
{
    
}

