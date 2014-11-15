#include <errno.h>
#include "fifo.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 256

char client_reader_name[PATH_MAX];
char client_writer_name[PATH_MAX];
char server_fifo_name[PATH];

int server_fifo_fd = -1;
int client_writer_fd = -1;
int client_reader_fd = -1;

void delete_fifos(void);
void pid_to_char(int pid, char **out);
int str_starts_with(char *str, char *str2); // http://stackoverflow.com/questions/15515088/how-to-check-if-string-starts-with-certain-string-in-c

int str_starts_with(char *str, char *str2)
{
    if(strncmp(str, str2, strlen(str)) == 0)
        return 1;
    return 0;    
}

void delete_fifos(void)
{
    unlink(client_reader_name);
    unlink(client_writer_name);    
}

void pid_to_char(int pid, char **out)
{
    short first, second;    
}

int main(int argc, char **argv)
{
    char read_buf[BUF_SIZE];
    int size_read = -1;

    umask(0);

    CREATE_SERVER_FIFO_NAME(server_fifo_name, (int)getpid());
    CREATE_CLIENT_READER_NAME(client_reader_name, (int)getpid());
    CREATE_CLIENT_WRITER_NAME(client_writer_name, (int)getpid());
    
    mkfifo(client_reader_name, FIFO_PERMISSIONS);
    mkfifo(client_writer_name, FIFO_PERMISSIONS);
    
    client_reader_fd = open(client_reader_name, O_RONLY);
    client_writer_fd = open(client_writer_name, O_WRONLY);

    atexit(delete_fifos);
    
    char pid_str[10];
    res = sprintf(pid_str, "%d");
    if(res == -1)
    {
        //print error    
        return -1;
    }
     
    res = write(server_fifo_fd, pid_str, res);     
    if(res == -1)
    {
        //print error
        return -1;    
    }
    
    while(1)
    {
        //read from the command line
        memset(read_buf, 0, BUF_SIZE);
        res = read(STDIN_FILENO, read_buf, BUF_SIZE);
        if(res == -1)
        {
            //print error
            return -1;    
        }
        
        size_read = res;

        //write the command to the server
        res = write(client_writer_fd, read_buf, size_read);    
        if(res == -1)
        {
            //print error
            return -1;    
        }
        
        if(strncmp(read_buf, "ldir", size_read) == 0)
        {
            
        }
        else if(strncmp(read_buf, "lcd", size_read) == 0)
        {
            
        }
        else if(strncmp(read_buf, "lpwd", size_read) == 0)
        {
                
        }
        else if(strncmp(read_buf, "dir", size_read) == 0)
        {
            //read the server's response
             
        }
        else if(strncmp(read_buf, "cd ", 3) == 0)
        {
            
        }
        else if(strncmp(read_buf, "exit", size_read) == 0)
        {
                
        }
        else if(strncmp(read_buf, "pwd", size_read) == 0)
        {
            
        }
        else if(strncmp(read_buf, "help", size_read) == 0)
        {
                
        }
        //read the server's response
    }
}
