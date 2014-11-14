/*#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <error.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>*/
#include <sys/wait.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <utime.h>
#include <stdint.h>
#include <errno.h>
#include <dirent.h>
#include <assert.h>

int main(int argc, char *argv[])
{

    int res = 0;
    int pfd[2];
    int pfd2[2];
    int pfd3[2];
    int pfd4[2];

    res = pipe(pfd);
    res = pipe(pfd2);
    res = pipe(pfd3);
    res = pipe(pfd4);

    switch(fork())
    {
        /*case -1:
           error = errno;
           printf("fork error(1): %s\n", strerror(error));
           return -1;
           break;
        */
         
        case 0:
            if(pfd[1] != STDOUT_FILENO)
            {
                res = dup2(pfd[1], STDOUT_FILENO);
            }

            res = close(pfd[0]);
            res = close(pfd2[0]);
            res = close(pfd3[0]);
            res = close(pfd4[0]);
            res = close(pfd[1]);
            res = close(pfd2[1]);
            res = close(pfd3[1]);
            res = close(pfd4[1]);
        	
            //remove_non_alpha(arg_str);	
        	//res = write(pfd[1], arg_str, len_arg_str);
            //f(res == -1)
            //	return -1;

            execlp("sed", "sed", "s/[^a-zA-Z]/ /g", (char *)NULL);
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
        case 0:
            if(pfd2[1] != STDOUT_FILENO)
            {
                res = dup2(pfd2[1], STDOUT_FILENO);
            }
            
            if(pfd[0] != STDIN_FILENO)
            {
                res = dup2(pfd[0], STDIN_FILENO);
            }

            res = close(pfd[0]);
            res = close(pfd2[0]);
            res = close(pfd3[0]);
            res = close(pfd4[0]);
            res = close(pfd[1]);
            res = close(pfd2[1]);
            res = close(pfd3[1]);
            res = close(pfd4[1]);
        	

            execlp("tr", "tr", "[A-Z]", "[a-z]", (char *)NULL);
            break; 
        default:
            break;
    }
    
    switch(fork())
    {
        case 0:
            if(pfd3[1] != STDOUT_FILENO)
            {
                res = dup2(pfd3[1], STDOUT_FILENO);
            }
            
            if(pfd2[0] != STDIN_FILENO)
            {
                res = dup2(pfd2[0], STDIN_FILENO);
            }

            res = close(pfd[0]);
            res = close(pfd2[0]);
            res = close(pfd3[0]);
            res = close(pfd4[0]);
            res = close(pfd[1]);
            res = close(pfd2[1]);
            res = close(pfd3[1]);
            res = close(pfd4[1]);
        	
            execlp("awk", "awk", "{ for (i = 1; i <= NF; i++) { print $i; } }", (char *)NULL);
            break; 
        default:
            break;
    }
    
    switch(fork())
    {
        case 0:
            if(pfd4[1] != STDOUT_FILENO)
            {
                res = dup2(pfd4[1], STDOUT_FILENO);
            }
            
            if(pfd3[0] != STDIN_FILENO)
            {
                res = dup2(pfd3[0], STDIN_FILENO);
            }

            res = close(pfd[0]);
            res = close(pfd2[0]);
            res = close(pfd3[0]);
            res = close(pfd4[0]);
            res = close(pfd[1]);
            res = close(pfd2[1]);
            res = close(pfd3[1]);
            res = close(pfd4[1]);
        	
            execlp("sort", "sort", (char *)NULL);
            break; 
        default:
            break;
    }
    
    switch(fork())
    {
        case 0:
            
            if(pfd4[0] != STDIN_FILENO)
            {
                res = dup2(pfd4[0], STDIN_FILENO);
            }

            res = close(pfd[0]);
            res = close(pfd2[0]);
            res = close(pfd3[0]);
            res = close(pfd4[0]);
            res = close(pfd[1]);
            res = close(pfd2[1]);
            res = close(pfd3[1]);
            res = close(pfd4[1]);
        	
            execlp("uniq", "uniq", "-c", (char *)NULL);
            break; 
        default:
            break;
    }

    res = close(pfd[0]);
    res = close(pfd2[0]);
    res = close(pfd3[0]);
    res = close(pfd4[0]);
    res = close(pfd[1]);
    res = close(pfd2[1]);
    res = close(pfd3[1]);
    res = close(pfd4[1]);

    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}
