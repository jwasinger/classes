#define _GNU_SOURCE
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
#include <stdio.h>

#include "DynArr.h"

void remove_non_alpha(char *str);
void lower_str(char *str);
void split_lines(char *str);
int read_arg(char **out_str, int *size_read, char *arg);
int read_all(int fd, char **out_str, int *out_mem_size);
int validate_exit_status(int status);

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

void split_lines(char *str)
{
    int i = 0;
    for(i=0; i<strlen(str); i++)
    {
        if(str[i] == ' ')
            str[i] = '\n';
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

int validate_exit_status(int status)
{
	char err_str[64];

	if(WIFEXITED(status))
	{
		if(WEXITSTATUS(status) != 0)
		{
			sprintf(err_str, "Process returned with unusual exit status: %s\n", WEXITSTATUS(status));
			fputs(err_str, stderr);
			return -1;
		}

		return 0;
	}
	else
	{
		sprintf(err_str, "Process terminated abnormally\n");
		fputs(err_str, stderr);
		return -1;
	}
}

int main(int argc, char **argv)
{
    char *arg_str;
    int len_arg_str = 0;

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
    
    if(argc == 1)
    {
        return -1;
        //res = read(STDIN_FILENO, &arg_str, &len_arg_str);
        if(res == -1)
        {
            error = errno;
            printf("read error(1): %s\n", strerror(error));
            return -1;
        }
    }

    else if(argc == 2)
    {
        res = read_arg(&arg_str, &len_arg_str, argv[1]);
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
    
    /*res = write(pfd[1], arg_str, arg_str_size);
    if(res < arg_str_size)
    {
    	printf("Error!!!\n");
    	return -1;
    } */ 

    switch(fork())
    {
        case -1:
           /*error case*/
           error = errno;
           printf("fork error(1): %s\n", strerror(error));
           return -1;
           break;
        
        case 0:

        	/*if(pfd2[1] != STDOUT_FILENO)
            {
	            res = dup2(pfd2[1], STDOUT_FILENO);
	            if(res == -1)
	            {
	           		return -1;
	            }
	            close(pfd2[1]);
            }

            if(pfd[0] != STDIN_FILENO)
            {
	            res = dup2(pfd[0], STDIN_FILENO);
				if(res == -1)
	            {
	           		return -1;
	            }
	            close(pfd[0]);
        	}*/

        	remove_non_alpha(arg_str);	
        	res = write(pfd[1], arg_str, len_arg_str);
            if(res == -1)
            	return -1;

	        return -1;
            break;

        default:
            //wait(&status);  
            //if(validate_exit_status(status) == -1)
            //{
            //	return -1;
            //}  
            /*parent case-- fall through*/ 
            break;
    }

    switch(fork())
    {
        case -1:
           /*error case*/
           error = errno;
           printf("fork error(1): %s\n", strerror(error));
           break;
        
        case 0: //child case: execute remove_non_alpha and send result to pfd write end
            
        	/*if(pfd3[1] != STDOUT_FILENO)
            {
	            res = dup2(pfd3[1], STDOUT_FILENO);
	            if(res == -1)
	            {
	           		return -1;
	            }
	            close(pfd3[1]);
            }

            if(pfd2[0] != STDIN_FILENO)
            {
	            res = dup2(pfd2[0], STDIN_FILENO);
				if(res == -1)
	            {
	           		return -1;
	            }
	            close(pfd2[0]);
        	}*/
	        res = read(pfd[0], arg_str, len_arg_str);
	        if(res < len_arg_str)
	        	return -1;

	        lower_str(arg_str);

	        res = write(pfd2[1], arg_str, len_arg_str);
	        if(res < len_arg_str)
	        	return -1;

            //execlp("cat", "cat", (char *)NULL);
            return 0;
            break;

        default:
            //wait(&status);    
            //validate_exit_status(status);
            /*parent case-- fall through*/ 
            break;
    }

    switch(fork())
    {
        case -1:
           /*error case*/
           error = errno;
           printf("fork error(1): %s\n", strerror(error));
           break;
        
        case 0: //child case: execute remove_non_alpha and send result to pfd write end
            
        	/*if(pfd4[1] != STDOUT_FILENO)
            {
	            res = dup2(pfd4[1], STDOUT_FILENO);
	            if(res == -1)
	            {
	           		return -1;
	            }
	            close(pfd4[1]);
            }

            if(pfd3[0] != STDIN_FILENO)
            {
	            res = dup2(pfd3[0], STDIN_FILENO);
				if(res == -1)
	            {
	           		return -1;
	            }
	            close(pfd3[0]);
        	}

            execlp("cat", "cat", (char *)NULL);*/
            res = read(pfd2[0], arg_str, len_arg_str);
            if(res < len_arg_str)
            	return -1;

            split_lines(arg_str);

            res = write(pfd3[1], arg_str, len_arg_str);
            if(res < len_arg_str)
            	return -1;

            return 0;
            break;

        default:
            //wait(&status);    
            //validate_exit_status(status);
            /*parent case-- fall through*/ 
            break;
    }

    switch(fork())
    {
        case -1:
           /*error case*/
           error = errno;
           printf("fork error(1): %s\n", strerror(error));
           break;
        
        case 0: //child case: execute remove_non_alpha and send result to pfd write end

        	if(pfd4[1] != STDOUT_FILENO)
            {
	            //res = dup2(pfd4[1], STDOUT_FILENO);
	            res = dup2(pfd4[1], STDOUT_FILENO);
	            if(res == -1)
	           		return -1;
            }

            if(pfd3[0] != STDIN_FILENO)
            {
	            res = dup2(pfd3[0], STDIN_FILENO);
				if(res == -1)
	            {
	           		return -1;
	            }
        	}
            
            close(pfd[0]);
            close(pfd[1]);
            close(pfd2[0]);
            close(pfd2[1]);
            close(pfd3[0]);
            close(pfd3[1]);
            close(pfd4[0]);
            close(pfd4[1]);
            close(pfd5[0]);
            close(pfd5[1]);

            execlp("sort", "sort", (char *)NULL);
            return -1;
            break;

        default:
            //wait(&status);    
            //validate_exit_status(status);
            /*parent case-- fall through*/ 
            break;
    }

    res = read(pfd4[0], arg_str, 1);
    if(res == -1)
    {
    	printf("asdfasdf");
    	return -1;
    }
    printf(arg_str);
    return 0;

    switch(fork())
    {
        case -1:
           /*error case*/
           error = errno;
           printf("fork error(1): %s\n", strerror(error));
           break;
        
        case 0: //child case: execute remove_non_alpha and send result to pfd write end
            
        	if(pfd5[1] != STDOUT_FILENO)
            {
	            res = dup2(pfd5[1], STDOUT_FILENO);
	            if(res == -1)
	            {
	           		return -1;
	            }
	            close(pfd5[1]);
            }

            if(pfd4[0] != STDIN_FILENO)
            {
	            res = dup2(pfd4[0], STDIN_FILENO);
				if(res == -1)
	            {
	           		return -1;
	            }
	            close(pfd4[0]);
        	}

            execlp("uniq", "uniq", "-c", (char *)NULL);
            return -1;
            break;

        default:
            //wait(&status);    
            //validate_exit_status(status);
            /*parent case-- fall through*/ 
            break;
    }

    res = read(pfd5[0], arg_str, len_arg_str);
    printf(arg_str);

    return 0;
    /*res = read_all(pfd6[0], &out_str, &out_len_str);
    if(res == -1)
    {
        printf("error");
        return -1;
    }
    printf(out_str);*/

}
