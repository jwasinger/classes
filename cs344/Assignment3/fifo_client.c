#include <errno.h>
#include "fifo.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 256

char client_reader_name[PATH_MAX];
char client_writer_name[PATH_MAX];
char server_fifo_name[PATH_MAX];

int server_fifo_fd = -1;
int client_writer_fd = -1;
int client_reader_fd = -1;

void delete_fifos(void);
void pid_to_char(int pid, char **out);
int str_starts_with(char *str, char *str2); // http://stackoverflow.com/questions/15515088/how-to-check-if-string-starts-with-certain-string-in-c
void print_error(void);
void strip_char(char *str, char c);


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

void print_error(void)
{
    int error = errno;
    printf("Error: %s\n", strerror(error));    
}


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

int main(int argc, char **argv)
{
    char read_buf[BUF_SIZE];
    char tmp_buf[BUF_SIZE];
    char path_buf[PATH_MAX];
    int size_read = -1;
    int res = 0;
    char pid_str[10];
    memset(pid_str, 0, 10);

    umask(0);

    CREATE_SERVER_FIFO_NAME(server_fifo_name);
    CREATE_CLIENT_READER_NAME(client_reader_name, (int)getpid());
    CREATE_CLIENT_WRITER_NAME(client_writer_name, (int)getpid());
    
    res = mkfifo(client_reader_name, FIFO_PERMISSIONS);
    res = mkfifo(client_writer_name, FIFO_PERMISSIONS);
    atexit(delete_fifos);
    FILE *fp = NULL;

    res = sprintf(pid_str, "%d", getpid());
    if(res == -1)
    {
        //print error    
        return -1;
    }

    server_fifo_fd = open(server_fifo_name, O_WRONLY);
    if(server_fifo_fd == -1)
    {
        //print error
        print_error();
        return -1;
    }     

    res = write(server_fifo_fd, pid_str, res);     
    if(res == -1)
    {
        //print error
        print_error();
        return -1;    
    }

    client_reader_fd = open(client_reader_name, O_RDONLY);
    client_writer_fd = open(client_writer_name, O_WRONLY);

    while(1)
    {
        //read from the command line
        memset(read_buf, 0, BUF_SIZE);
        /*res = read(STDIN_FILENO, read_buf, BUF_SIZE);
        if(res == -1)
        {
            //print error
            return -1;    
        }*/
        fgets(read_buf, BUF_SIZE, stdin);

        //write the command to the server
        res = write(client_writer_fd, read_buf, BUF_SIZE);    
        if(res == -1)
        {
            //print error
            print_error();
            return -1;    
        }
        
        if(strncmp(read_buf, "exit", 4) == 0)
        {
            return 0;
        }
        else if(strncmp(read_buf, "ldir", 4) == 0)
        {
            fp = popen(CMD_LS_POPEN, "r");
            if(fp == NULL)
            {
                printf("error");
                return -1;
            }

            printf("client process directory listing: \n");
            while(fgets(path_buf, PATH_MAX, fp))
                printf("%s", path_buf);

            continue;
        }
        else if(strncmp(read_buf, "lcd ", 4) == 0)
        {
            char *new_dir = read_buf + 4;
            strip_char(new_dir, '\n');

            res = chdir(new_dir);
            if(res == -1)
            {
                if(errno == ENOENT)
                {
                    printf("no such file or directory");
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

            printf("new directory for client process: %s\n", tmp_buf);
            continue;
        }
        else if(strncmp(read_buf, "lpwd", 4) == 0)
        {
            getcwd(tmp_buf, PATH_MAX);
            printf("client pwd: %s\n", tmp_buf);
            continue;
        }
        else if(strncmp(read_buf, "dir", 3) == 0)
        {
            res = read(client_reader_fd, tmp_buf, BUF_SIZE);
            if(res == -1)
            {
                print_error();
                return -1;
            }

            printf(tmp_buf);
        }
        else if(strncmp(read_buf, "cd ", 3) == 0)
        {
            //read the server's response
            res = read(client_reader_fd, tmp_buf, BUF_SIZE);
            if(res == -1)
            {
                print_error();
                return -1;
            }
            else if(res == 0)
            {
                printf("no response from the server\n");
                return -1;
            }

            printf("server pwd: %s\n", tmp_buf);
        }
        else if(strncmp(read_buf, "exit", size_read) == 0)
        {
            exit(0);
        }
        else if(strncmp(read_buf, "pwd", 3) == 0)
        {
            res = read(client_reader_fd, tmp_buf, BUF_SIZE);
            if(res == -1)
            {
                print_error();
                return -1;
            }

            printf("server pwd is: %s\n", tmp_buf);
            continue;
        }
        else if(strncmp(read_buf, "help", size_read) == 0)
        {
            printf("commands are: dir, pwd, ldir, lpwd, cd, lcd\n");
            continue;
        }
        //read the server's response
    }
}
