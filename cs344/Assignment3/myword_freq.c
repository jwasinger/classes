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
int read_arg(char **out_str, int *size_read, char *arg);
int read_all(int fd, char **out_str, int *out_mem_size);

//this function is buggy fasho
int read_all(int fd, char **out_str, int *out_mem_size)
{
    int num_read = 0;
    struct DynArr *dyn_arr = create_dyn_array(256, sizeof(char));
    char read_buf[256];
    int i = 0;
    int error = 0;

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

    *out_str = malloc(sizeof(char) * dyn_arr->size_array + 1);
    memcpy(*out_str, dyn_arr->array, dyn_arr->size_array);
    out_str[dyn_arr->size_array] = '\0';
    *out_mem_size = dyn_arr->size_array + 1;
    free_dyn_array(&dyn_arr);

    return 0;
}

int read_arg(char **out_str, int *size_read, char *arg)
{
    int res = 0;
    int error = 0;
    int fd = 0;

    res = open(arg, O_RDONLY, 0);
    if(res == -1)
    {
        if(errno != ENOENT)
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
        read_all(fd, out_str, size_read);        
        return 0;
    }
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
    int pfd2[2];
    int pfd3[2];
    int pfd4[2];
    
    char *str = "hello world\n";
    char *arg_str = NULL;
    int arg_str_size = 0;
    
    res = pipe(pfd);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(1): %s\n", strerror(error));    
    }
    
    res = pipe(pfd2);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(2): %s\n", strerror(error));    
        return -1;
    }
    
    res = pipe(pfd3);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(3): %s\n", strerror(error));    
    }
   
    res = pipe(pfd4);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(4): %s\n", strerror(error));    
        return -1;
    }
    
    res = pipe(pfd5);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(5): %s\n", strerror(error));    
    }

    //link end of 
    switch(fork())
    {
        case -1:
           /*error case*/
           error = errno;
           printf("fork error(1): %s\n", strerror(error));
           break;
        
        case 0:
            res = write(pfd[1], (void *)str, 13);
            if(res < 13)
            {
                return -1;
            }
            
            break;

        default:
            /*parent case-- fall through*/ 
            break;
    }

    
    char *read_pfd = NULL;
    int size_read_pfd = 0;
    
    switch(fork())
    {
        case -1:
            /*error case*/ 
        case 0:


            /*child case*/ 
            //pass data from read descriptor of pfd to lowerCase
            res = read_all(pfd[0], &read_pfd, &size_read_pfd);
            
            read_pfd[0] = 'a';
            
            //write the output to the write descriptor of pfd2
            res = write(pfd2[1], read_pfd, size_read_pfd);
        default:
            /*parent case*/
            break;
    }
    
    char *pfd2_output = NULL;
    int pfd2_output_len = 0;
    res = read_all(pfd2[0], &pfd2_output, &pfd2_output_len); 

    switch(fork())
    {
        case -1:
            error = errno;
            printf("Fork error(3): %s\n", strerror(error));
            break;
        case 0:
            break;
        default:
            break;
    }
    
    switch(fork())
    {
        case -1:
            error = errno;
            printf("Fork error(3): %s\n", strerror(error));
            break;
        case 0:
            break;
        default:
            break;
    }
    
    /*switch(fork())
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
    
    res = pipe(pfd);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(2): %s\n", strerror(error));
    }

    switch(fork())
    {
        case -1:
            error = errno;
            printf("fork error(2): %s\n", strerror(error));
            return -1;
        case 0:
            
            break;
        default: // parent
            
            break;
    }
    printf("done!");*/
    return 0;
}
