#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "util/DynArr.h"

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
    int write_pos = 0;

    for(i = 0; i < strlen(str); i++)
    {
        if(str[i] == '\n')
        {
            line_size = line_arr->size_array;
            line = malloc(sizeof(char) * line_size);
            memcpy(line, line_arr->array, line_size);

            free_dyn_arr(line_arr);
            line_arr = create_dyn_arr(10, sizeof(char));
            
            out_lines[line_num] = line;

            write_pos = 0;
            line_num++;
            continue;
        }
        
        add_dyn_array(line_arr, &(str[i]));
    }

    //copy last line
    line_size = line_arr->size_array;
    line = malloc(sizeof(char) * line_size);
    memcpy(line, line_arr->array, line_size);

    free_dyn_arr(line_arr);
}

int main(int argc, char **argv)
{
    int res = 0;
    int error;
    int pfd[2];

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
            
            //close read file descriptor
            res = close(pfd[0]);
            if(res == -1)
            {
                error = errno;
                printf("error, close(1): %s\n", strerror(error));
            }
            
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
            
            execlp("ls", "ls", (char *)NULL);
            
            printf("execlp ls");
            return -1;

            break;

        case -1:
            error = errno;
            printf("fork error: %s\n", strerror(error));
            break;

        default: 
            break;
    }

    switch(fork())
    {
        case -1:
            error = errno;
            printf("fork error: %s\n", strerror(error));
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
