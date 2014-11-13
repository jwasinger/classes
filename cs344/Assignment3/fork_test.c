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

int main(int argc, char **argv)
{
    char *arg_str;
    int len_arg_str = 0;


    int res = 0;
    int error;
    int pfd[2];
    int pfd2[2];
    int pfd3[2];
    int pfd4[2];

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
        printf("pipe error(3): %s\n", strerror(error));    
    }

    switch(fork())
    {
        case -1:
           /*error case*/
           error = errno;
           printf("fork error(1): %s\n", strerror(error));
           return -1;
           break;
        
        case 0:
            if(pfd[1] != STDOUT_FILENO)
            {
                dup2(pfd[1], STDOUT_FILENO);
            }

            close(pfd[0]);
            close(pfd[1]);
            close(pfd2[0]);
            close(pfd2[1]);
            close(pfd3[0]);
            close(pfd3[1]);
            close(pfd4[0]);
            close(pfd4[1]);

        	//remove_non_alpha(arg_str);	
        	//res = write(pfd[1], arg_str, len_arg_str);
            //f(res == -1)
            //	return -1;

            execlp("sed", "sed", "s/[^a-zA-Z]/ /g", (char *)NULL);
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

    return 0;

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
	        
            if(pfd[0] != STDIN_FILENO)
            {
                dup2(pfd[0], STDIN_FILENO);
            }

            if(pfd2[1] != STDOUT_FILENO)
            {
                dup2(pfd2[1], STDOUT_FILENO);
            }

            close(pfd[0]);
            close(pfd[1]);
            close(pfd2[0]);
            close(pfd2[1]);
            close(pfd3[0]);
            close(pfd3[1]);
            close(pfd4[0]);
            close(pfd4[1]);

            execlp("tr", "tr", "[A-Z]", "[a-z]", (char *)NULL);
            //execlp("cat", "cat", (char *)NULL);
            return 0;
            break;

        default:
            //wait(&status);    
            //validate_exit_status(status);
            /*parent case-- fall through*/ 
            break;
    }

    return 0;

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
            

            execlp("uniq", "uniq", "-c", (char *)NULL);
            return -1;
            break;

        default:
            //wait(&status);    
            //validate_exit_status(status);
            /*parent case-- fall through*/ 
            break;
    }

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
