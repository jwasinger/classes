#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <error.h>
#include <errno.h>

#include "DynArr.h"

/* Prototypes */
void remove_non_alpha(char *str);
void lower_str(char *str);
void split_lines(char *str, char **out_lines, int *out_num_lines);
int read_arg(char **out_str, char *arg);
int read_all(char **out_str, int fd);

int read_arg(char **out_str, char *arg)
{
    int res = 0;
    int fd = 0;
    char read_buf[256];
    int num_read = 0;
    int error = 0;
    int i = 0;
    struct DynArr *dyn_arr = create_dyn_array(256,sizeof(char));
    
    res = open(arg, O_RDONLY, 0);
    if(res == -1)
    {
        free_dyn_array(&dyn_arr);
        if(errno == ENOENT)
        {
            error = errno;
            printf("open error(1): %s\n", strerror(error));
            return -1;
        }

        return 1; 
    }
    else
    {
        fd = res;
        
        while((num_read = read(fd, read_buf, 256)) == 256)
        {
            for(i = 0; i < 256; i++)
            {
                add_dyn_array(dyn_arr, (void *)&read_buf[i]);
            }
        }

        if(num_read == -1)
        {
            error = errno;
            printf("read error(1): %s\n", strerror(error));
        }
        else //copy the last of what was read to the dynarr
        {
            for(i = 0; i < num_read; i++)
            {
                add_dyn_array(dyn_arr, (void *)&read_buf[i]);
            }
        }

        *out_str = malloc(sizeof(char) * dyn_arr->size_array);
        memcpy(*out_str, dyn_arr->array, dyn_arr->size_array);
        free_dyn_array(&dyn_arr);
    }

    return 0;
}

void remove_non_alpha(char *str)
{
    int len = strlen(str);
    int i = 0;
    
    for(i = 0; i < len; i++)
    {
        if(str[i] < 'A' || str[i] > 'z')
            str[i] = ' ';
    }
}

void lower_str(char *str)
{
    int len = strlen(str);
    int i = 0;

    for(i = 0; i < len; i++)
    {
        str[i] = tolower(str[i]);
    }
}

void split_lines(char *str, char **out_lines, int *out_num_lines)
{
    int i = 0;
    struct DynArr *line_arr = create_dyn_array(10, sizeof(char));
    int line_num = 0;
    int line_size = 0;
    char *line = NULL;

    for(i = 0; i < strlen(str); i++)
    {
        if(str[i] == '\n')
        {
            line_size = line_arr->size_array;
            line = malloc(sizeof(char) * line_size);
            memcpy(line, line_arr->array, line_size);

            free_dyn_array(&line_arr);
            line_arr = create_dyn_array(10, sizeof(char));
            
            out_lines[line_num] = line;

            line_num++;
            continue;
        }
         
        add_dyn_array(line_arr, &(str[i]));
    }

    //copy last line
    line_size = line_arr->size_array;
    line = malloc(sizeof(char) * line_size);
    memcpy(line, line_arr->array, line_size);

    free_dyn_array(&line_arr);
}

int main(int argc, char **argv)
{
    int res = 0;
    int error;
    int pfd[2];
    char *out_str = NULL;
    
    //try and open the first command line argument as a file, if that fails, pass it as text
    /*if(argc < 2)
    {
        printf("not enough arguments supplied.... exiting\n");
        return -1;
    }
    
    res = read_arg(&out_str, argv[1]);
    if(res == 1)
    {
        //pass argv[1] to the pipe
    }
    else if (res == 0)
    {
        //pass out_str to the pipe
    } */

    res = pipe(pfd);
    if(res == -1)
    {
        error = errno;
        printf("pipe error: %s\n", strerror(error));    
    }

    switch(fork())
    {
        case 0:
            /* child process */
            
            /*
            //close read file descriptor
            res = close(pfd[0]);
            if(res == -1)
            {
                error = errno;
                printf("error, close(1): %s\n", strerror(error));
            }
            */
            
            if(pfd[1] != STDOUT_FILENO)
            {
                if(dup2(pfd[1], STDOUT_FILENO) == -1)
                {
                    error = errno;
                    printf("error dup2(1): %s\n", strerror(error));
                }
                
                if(close(pfd[1]) == -1)
                {
                    error = errno;
                    printf("error close(2): %s\n", strerror(error));
                }
            }
            
            //remove_non_alpha( 
            //execlp("ls", "ls", "-l", (char *)NULL); 
            printf("Parent!\npoop\n1sdfa\n");

            return -1;

            break;

        case -1:
            error = errno;
            printf("fork error(1): %s\n", strerror(error));
            break;

        default: //parent, write
            break;
    }

    switch(fork())
    {
        case -1:
            error = errno;
            printf("fork error(2): %s\n", strerror(error));
            break;

        case 0:
            
            res = close(pfd[1]);
            if(res == -1)
            {
                error = errno;
                printf("error, close(3): %s\n", strerror(error));
            }
            
            if(pfd[0] != STDIN_FILENO)
            {
                if(dup2(pfd[0], STDIN_FILENO) == -1)
                {
                    error = errno;
                    printf("error dup2(2): %s\n", strerror(error));
                }
                
                if(close(pfd[0]) == -1)
                {
                    error = errno;
                    printf("error close(4): %s\n", strerror(error));
                }
            }

            execlp("wc", "wc", "-l", (char *)NULL);

            error = errno;
            printf("error execlp wc: %s\n", strerror(error));
            break;

        default:
            break;
    }

    printf("done!");
    return 0;
}
