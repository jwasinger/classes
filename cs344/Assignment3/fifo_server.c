#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "fifo.h"

#define BUF_SIZE 256 

char server_fifo_name[PATH_MAX];
char client_reader_name[PATH_MAX];
char client_writer_name[PATH_MAX];
char path_buf[PATH_MAX];

void strip_char(char *str, char c);
int pid_str_to_int(char *str); // grab the first two characters from str and convert them into an int (the process id)
void print_error(void);
void child_proc_exit(int sig);

void strip_char(char *str, char c)
{
    int size = strlen(str);
    int i = 0;

    for(; i < size; i++)
    {
        if(str[i] == c)
            str[i] = '\0'; // a terrible unforgivable hack ;)
    }
}

//http://stackoverflow.com/questions/1294649/cleanest-way-to-combine-two-shorts-to-an-int
int pid_str_to_int(char *str)
{
    return atoi(str);
}

void child_proc_exit(int sig)
{
    wait(NULL);
}

static void my_exit(void)
{
    unlink(server_fifo_name);
    unlink(client_reader_name);
    unlink(client_writer_name);
}

void print_error(void)
{
    int error = errno;
    printf("Error: %s\n", strerror(error));    
}

int main(int argc, char **argv)
{
    int res = 0;
    
    char msg_from_client[256];
    char tmp_buf[BUF_SIZE];

    int server_read_fd = -1;
    //int server_write_fd = -1;
    int client_read_fd = -1;
    int client_write_fd = -1;
    FILE *fp = NULL;

    umask(0);

    //create the server fifo
    CREATE_SERVER_FIFO_NAME(server_fifo_name);
    res = mkfifo(server_fifo_name, FIFO_PERMISSIONS);
    atexit(my_exit);

    server_read_fd = open(server_fifo_name, O_RDONLY);

    //server_write_fd = open(server_fifo_name, O_WRONLY);

    //signal(SIGPIPE, SIG_IGN);
    //signal(SIGINT, exit_handler)
    
    while(1)
    {
        //memset(client)
        res = read(server_read_fd, &msg_from_client, 256);
        if(res == -1)
        {
            //exit here    
            return -1;
        }
        if(res != 0)
        {
            printf("client connected\n");
            //set the child exit handler to make the parent wait until child is fully closed
            
            signal(SIGCHLD, child_proc_exit);
            switch(fork())
            {
                case -1:
                    //report an error
                    break;
                case 0:

                    CREATE_CLIENT_READER_NAME(client_reader_name, pid_str_to_int(msg_from_client));
                    CREATE_CLIENT_WRITER_NAME(client_writer_name, pid_str_to_int(msg_from_client));
                    
                    client_read_fd = open(client_reader_name, O_WRONLY);
                    client_write_fd = open(client_writer_name, O_RDONLY);
                    
                    while(1)
                    {
                        res = read(client_write_fd, msg_from_client, BUF_SIZE);
                        if(res == -1)
                        {
                            printf("Error!\n");
                            return -1;
                        }
                        if(res == 0)
                        {
                            continue;
                        }

                        if(strncmp(msg_from_client, "exit", 4) == 0)
                            break; 
                    
                        
                        if(strncmp(msg_from_client, "cd", 2) == 0)
                        {
                            char *new_dir = msg_from_client + 3;
                            strip_char(new_dir, '\n');

                            res = chdir(new_dir);
                            if(res == -1)
                            {
                                if(errno == ENOENT)
                                {
                                    res = write(client_read_fd, "No such file or directory\n\0", 27);
                                    continue;
                                }
                                print_error();
                                return -1;                                
                            }
                            
                            if(getcwd(tmp_buf, BUF_SIZE) == NULL)
                            {
                                print_error();
                                return -1;
                            }

                            res = write(client_read_fd, tmp_buf, BUF_SIZE);
                            if(res == -1)
                            {
                                print_error();
                                return -1;    
                            }

                        }
                        else if(strcmp(msg_from_client, "dir") == 0)
                        {
                            fp = popen(CMD_LS_POPEN, "r");
                            if(fp == NULL)
                            {
                                printf("error");
                                return -1;
                            }

                            while(fgets(path_buf, PATH_MAX, fp))
                                sprintf(tmp_buf, "%s", path_buf);

                            res = write(client_read_fd, tmp_buf, PATH_MAX);
                            if(res == -1)
                            {
                                print_error();
                                return -1;
                            }
                        }
                        else if(strncmp(msg_from_client, "pwd", 3) == 0)
                        {
                            memset(tmp_buf, 0, BUF_SIZE);
                            getcwd(tmp_buf, BUF_SIZE);
                            write(client_read_fd, tmp_buf, BUF_SIZE);                     
                        }
                        else if(strcmp(msg_from_client, "help") == 0)
                        {
                            continue;
                        }
                        else
                        {
                            //error message!!!    
                        }

                        memset(msg_from_client, 0, BUF_SIZE);
                        memset(tmp_buf, 0, BUF_SIZE);
                    }

                    close(client_write_fd);
                    close(client_read_fd);
                    //clean up 

                    printf("client disconnected\n");
                    return 0;
                    break;
                default:
                    break;    
            }
        }
    }
}
