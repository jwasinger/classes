#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(char **argv, int argc)
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
            printf("fork error: %s\n", strerror(error);
            break;

        case 0:
            
            res = close(pfd[0]);
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
}
