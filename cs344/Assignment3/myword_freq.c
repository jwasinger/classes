#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <error.h>
#include <errno.h>
#include <sys/wait.h>

#include "DynArr.h"

void remove_non_alpha(char *str);
void lower_str(char *str);
void split_lines(char *str);
int read_arg(char **out_str, int *size_read, char *arg);
int read_all(int fd, char **out_str, int *out_mem_size);

int read_arg(char **out_str, int *size_read, char *arg)
{
    int res = 0;
    int error = 0;
    int fd = 0;
    int *stage = malloc(sizeof(int));

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

//this function is buggy fasho
int read_all(int fd, char **out_str, int *out_mem_size)
{
    int num_read = 0;
    struct DynArr *dyn_arr = create_dyn_array(256, sizeof(char));
    char read_char;
    int error = 0;
    

    while((num_read = read(fd, &read_char, 1)) == 1)
    {
        add_dyn_array(dyn_arr, (void *)&read_char);
    }

    if(num_read == -1)
    {
        if(errno != EWOULDBLOCK && errno != EAGAIN)
        {
        error = errno;
        printf("read error(1): %s\n", strerror(error));
        }
    }

    *out_str = malloc(sizeof(char) * dyn_arr->size_array + 1);
    memcpy(*out_str, dyn_arr->array, dyn_arr->size_array);
    out_str[dyn_arr->size_array] = '\0';
    *out_mem_size = dyn_arr->size_array + 1;
    free_dyn_array(&dyn_arr);

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

void split_lines(char *str)
{
    int i = 0;
    for(i=0; i<strlen(str); i++)
    {
        if(str[i] == ' ')
            str[i] = '\n';
    }
}

int main(int argc, char **argv)
{
    char *arg_str;
    int arg_str_size;

    int status = -1;

    //int len_arg_str = 6;
    
    char *read_pfd = NULL;
    int size_read_pfd = -1;

    char *out_str = NULL;
    int out_len_str = -1;

    int res = 0;
    int error;
    int pfd[2];
    int pfd2[2];
    int pfd3[2];
    int pfd4[2];
    int pfd5[2];
    int pfd6[2];
    
    res = pipe2(pfd, O_NONBLOCK);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(1): %s\n", strerror(error));    
    }
    
    res = pipe2(pfd2, O_NONBLOCK);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(2): %s\n", strerror(error));    
        return -1;
    }
    
    res = pipe2(pfd3, O_NONBLOCK);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(3): %s\n", strerror(error));    
    }
   
    res = pipe2(pfd4, O_NONBLOCK);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(4): %s\n", strerror(error));    
        return -1;
    }
    
    res = pipe2(pfd5, O_NONBLOCK);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(5): %s\n", strerror(error));    
    }
    
    res = pipe2(pfd6, O_NONBLOCK);
    if(res == -1)
    {
        error = errno;
        printf("pipe error(5): %s\n", strerror(error));    
    }
    
    if(argc == 1)
    {
        return -1;
        /*res = read(STDIN_FILENO, &arg_str, &len_arg_str);
        if(res == -1)
        {
            error = errno;
            printf("read error(1): %s\n", strerror(error));
            return -1;
        }*/
    }
    else if(argc == 2)
    {
        res = read_arg(&arg_str, &arg_str_size, argv[1]);
        if(res == -1)
        {
            error = errno;
            printf("read_arg error(2): %s\n", strerror(error));
            return -1;
        }
    }
    else
    {
        printf("error: invalid number of arguments\n");
        return -1;
    }
    
    switch(fork())
    {
        case -1:
           /*error case*/
           error = errno;
           printf("fork error(1): %s\n", strerror(error));
           break;
        
        case 0: //child case: execute remove_non_alpha and send result to pfd write end
            remove_non_alpha(arg_str);

            res = write(pfd[1], arg_str, arg_str_size);
            if(res != arg_str_size)
            {
                return -1;
            }
            
            return 0;
            break;

        default:
            wait(&status);    
            /*parent case-- fall through*/ 
            break;
    }

    switch(fork())
    {
        case -1:
            /*error case*/ 
        case 0:

            /*child case*/ 
            //pass data from read descriptor of pfd to lowerCase
            res = read_all(pfd[0], &read_pfd, &size_read_pfd);
            if(res == -1)
            {
                return -1;
            }
            
            lower_str(read_pfd);
            
            //write the output to the write descriptor of pfd2
            res = write(pfd2[1], read_pfd, size_read_pfd);
            if(res == -1)
            {
                return -1;
            }

            return 0;
            break;
        default:
            /*parent case*/
            wait(&status);
            break;
    }
    
    switch(fork())
    {
        case -1:
            error = errno;
            printf("Fork error(3): %s\n", strerror(error));
            break;
        case 0: // call splitlines and pass result to write end of pfd3
            res = read_all(pfd2[0], &read_pfd, &size_read_pfd);
            if(res == -1 )
            {
                return -1;
            }
            
            split_lines(read_pfd);
            
            //write the output to the write descriptor of pfd2
            res = write(pfd3[1], read_pfd, size_read_pfd);
            if(res == -1)
            {
                return -1;
            }

            return 0;
            break;
        default:
            wait(&status);
            break;
    }
    
    switch(fork())
    {
        case -1:
            error = errno;
            printf("Fork error(3): %s\n", strerror(error));
            break;
        case 0:
            //read from pfd3, call sort on what is read, pass the result of sort to write descriptor of pfd3 
            res = read_all(pfd3[0], &read_pfd, &size_read_pfd);
            if(res == -1)
            {
                return -1;
            }
            
            split_lines(read_pfd);
            
            //write the output to the write descriptor of pfd2
            res = write(pfd4[1], read_pfd, size_read_pfd);
            if(res == -1)
            {
                return -1;
            }
            
            return 0;
            break;
        default:
            wait(&status);
            if(!WIFEXITED(status))
            {
                return -1;
            }
            break;
    }
    
    switch(fork())
    {
        case -1:
            error = errno;
            printf("Fork error(3): %s\n", strerror(error));
            break;
        case 0:
            //read from pfd3, call sort on what is read, pass the result of sort to write descriptor of pfd4 
            /*res = read_all(pfd4[0], &read_pfd, &size_read_pfd);
            if(res == -1 )
            {
                return -1;
            }*/
           
            //call sort()
            res = dup2(pfd4[0], STDIN_FILENO);
            if (res == -1)
            {
                return -1;
            }
            //close(pfd4[0]);

            res = dup2(pfd5[1], STDOUT_FILENO);
            if (res == -1)
            {
                return -1;
            }
            //close(pfd5[1]);

            execlp("sort", "sort", (char *)NULL);

            //write the output to the write descriptor of pfd2
            res = write(pfd5[1], read_pfd, size_read_pfd);
            if(res == -1)
            {
                return -1;
            }

            return 0;
            break;
        default:
            wait(&status);
            if(!WIFEXITED(status))
            {
                return -1;
            }
            break;
    }
    
    switch(fork())
    {
        case -1:
            error = errno;
            printf("Fork error(3): %s\n", strerror(error));
            break;
        case 0:
            //read from pfd4, call uniq on what is read, pass the result of sort to write descriptor of pfd5 
            res = read_all(pfd5[0], &read_pfd, &size_read_pfd);
            if(res == -1)
            {
                return -1;
            }
           
            //call uniq -c
            execlp("uniq", "uniq", "-c", read_pfd, (char *)NULL);
            
            //write the output to the write descriptor of pfd5
            res = write(pfd6[1], read_pfd, size_read_pfd);
            if(res == -1)
            {
                return -1;
            }

            return 0;
            break;
            
        default:
            wait(&status);
            break;
    }


    res = read_all(pfd6[0], &out_str, &out_len_str);
    if(res == -1)
    {
        printf("error");
        return -1;
    }
    printf(out_str);

    return 0;
}
