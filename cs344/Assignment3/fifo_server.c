#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "fifo.h"

#define BUF_SIZE 1024 

int get_pid(char *str); // grab the first two characters from str and convert them into an int (the process id)

//http://stackoverflow.com/questions/1294649/cleanest-way-to-combine-two-shorts-to-an-int
int get_pid(char *str)
{
    short first_part = (short)str[0];
    short second_part = (short)str[1];    

    return ((first_part<<16) | ((second_part) & 0xffff));
}

void print_error(void)
{
    int error = errno;
    printf("Error: %s\n", strerror(error));    
}

int main(int argc, char **argv)
{
    int res = 0;
    char server_fifo_name[PATH_MAX];
    char reader_name[PATH_MAX];
    char writer_name[PATH_MAX];
    char client_reader_name[PATH_MAX];
    char client_writer_name[PATH_MAX];
    
    char msg_from_client[256];
    char tmp_buff[BUF_SIZE];

    int server_read_fd = -1;
    int server_write_fd = -1;
    int client_read_fd = -1;
    int client_write_fd = -1;
    //umask(0)

    //create the server fifo
    CREATE_SERVER_FIFO_NAME(server_fifo_name);
    res = mkfifo(server_fifo_name, FIFO_PERMISSIONS);

    server_read_fd = open(server_fifo_name, O_RDONLY);

    server_write_fd = open(server_fifo_name, O_WRONLY);

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
        
        //set the child exit handler to make the parent wait until child is fully closed
        
        switch(fork())
        {
            case -1:
                //report an error
                break;
            case 0:
                CREATE_CLIENT_READER_NAME(client_reader_name, get_pid(msg_from_client));
                CREATE_CLIENT_WRITER_NAME(client_writer_name, get_pid(msg_from_client));
                client_read_fd = open(client_writer_name, O_WRONLY);
                client_write_fd = open(client_reader_name, O_RDONLY);
                 
                while(strcmp(msg_from_client, "exit") != 0)
                {
                    if(strncmp(msg_from_client, "cd", 2) == 0)
                    {
                        char *new_dir = msg_from_client + 3;
                        res = chdir(new_dir);
                        if(res == -1)
                        {
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
                        }

                    }
                    else if(strcmp(msg_from_client, "dir") == 0)
                    {
                    
                    }
                    else if(strcmp(msg_from_client,  "pwd") == 0)
                    {
                        memcpy(tmp_buff, 0, BUF_SIZE);
                        getcwd(tmp_buff, BUF_SIZE);
                        write(client_read_fd, tmp_buff, BUF_SIZE);                     
                    }
                    else if(strcmp(msg_from_client, "help") == 0)
                    {
                            
                    }
                    else
                    {
                        //error message!!!    
                    }
                }

                //clean up 
                break;
            default:
                break;    
        }
    }
}
