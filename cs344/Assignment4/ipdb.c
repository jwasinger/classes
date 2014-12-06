#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "provided_materials/ipdb.h"

#define SHM_NAME "ipdb_shm"
#define BUF_SIZE 256

struct shm_db_hdr 
{
    sem_t db_lock;
    //int db_locked;
    int db_size;
    int num_rows;
};

int SHM_SIZE = sizeof(struct shm_db_hdr)+sizeof(ip_row_t)*MAX_ROWS;

int get_ip_strs(char *host, char *ip_v4, char *ip_v6);
int init_shm_db(int **addr);
int add_entry(char *host);
void print_row(const ip_row_t *row);
int show_rows(void);

static struct shm_db_hdr *hdr = NULL;
static void *data_addr = NULL;
int *shm_addr = NULL;

void print_row(const ip_row_t *row)
{
    printf("Name: %s\n", row->row_name);
    printf("Ipv6: %s\n", row->row_address6);
    printf("Ipv4: %s\n\n", row->row_address4);
}

int show_rows(void)
{
    int i = 0;
    int error = 0;
    int res = 0;
    int num_rows = 0;
    ip_row_t *row;

    /*res = sem_wait(&hdr->db_lock);
    if(res == -1)
    {
        error = errno;
        printf("sem_wait error: %s\n", strerror(error));
        return -1;
    }*/

    num_rows = hdr->num_rows;

    /*res = sem_post(&hdr->db_lock);
    if(res == -1)
    {
        error = errno;
        printf("sem_post error: %s\n", strerror(error));
        return -1;
    }*/

    for(i = 0; i < num_rows; i++)
    {
        row = (ip_row_t *)((int *)data_addr + num_rows*sizeof(ip_row_t));

        res = sem_wait(&row->row_lock);
        if(res == -1)
        {
            error = errno;
            printf("sem_wait error: %s\n", strerror(error));
            return -1;
        }

        print_row(row);

        res = sem_post(&row->row_lock);
        if(res == -1)
        {
            error = errno;
            printf("sem_post error: %s\n", strerror(error));
            return -1;
        }
    }

    return 0;
}

int init_shm_db(int **addr)
{
    int fd = -1;
    int res = 0;
    int error;
    struct shm_db_hdr *hdr;
    
    fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666);
    if(fd == -1)
    {
        error = errno;
        printf("shm_open error: %s", strerror(error));
        return -1;
    }

    res = ftruncate(fd, SHM_SIZE);
    if(res == -1)
    {
        error = errno;
        printf("ftruncate error: %s\n", strerror(error));
        return -1;
    }

    //map address and set up the header struct
    *addr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(*addr == MAP_FAILED)
    {
        error = errno;
        printf("error mapping shared memory: %s\n", strerror(error));
        return -1;
    }

    struct shm_db_hdr 
    {
        sem_t db_lock;
        //int db_locked;
        int db_size;
        int num_rows;
    };

    hdr = (struct shm_db_hdr *)*addr;
    
    res = sem_init(&hdr->db_lock, 1, 1);
    if(res == -1)
    {
        error = errno;
        printf("sem_init error: %s\n", strerror(error));
        return -1;
    }

    hdr->db_size = 0;
    hdr->num_rows = 0;

    return 0;
}

int get_ip_strs(char *host, char *ip_v4, char *ip_v6)
{
    struct addrinfo hints, *res, *p;
    int status;
    void *addr;
    struct sockaddr_in *ipv4 = NULL;
    struct sockaddr_in6 *ipv6 = NULL;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    if(!*ip_v4 || !*ip_v6)
    {
        printf("invalid args\n");
        return -1;
    }
    
    if ((status = getaddrinfo(host, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    for(p = res;p != NULL; p = p->ai_next) 
    {
        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (p->ai_family == AF_INET) 
        { // IPv4
            if(ipv4 != NULL)
                continue;
            ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            inet_ntop(p->ai_family, addr, ip_v4, NAME_SIZE);
        } 
        else 
        { // IPv6
            if(ipv6 != NULL)
                continue;
            ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            inet_ntop(p->ai_family, addr, ip_v6, NAME_SIZE);
        }

        if(ipv4 && ipv6)
            break;
    }

    return 0;
}

int add_entry(char *host)
{
    int res = 0;
    int *addr = (int *)data_addr;
    ip_row_t *last_row;
    int num_rows = 0;
    int error = 0;

    char ip_v4[NAME_SIZE];
    char ip_v6[NAME_SIZE];

    res = sem_wait(&hdr->db_lock);
    if(res == -1)
    {
        error = errno;
        printf("sem_wait error: %s\n", strerror(error));
        return -1;
    }

    /*if(hdr->db_locked)
    {
        sem_post(&hdr->db_lock);
        return 1;
    }*/

    res = get_ip_strs(host, ip_v4, ip_v6);
    if(res == -1)
    {
        errno = error;
        printf("get_ip_strs error: %s\n", strerror(error));
        return -1;
    }

    num_rows = hdr->num_rows;
    hdr->num_rows++;
    sem_post(&hdr->db_lock);
    
    last_row = (ip_row_t *)(addr + num_rows*sizeof(ip_row_t));
    memset(last_row, 0, sizeof(ip_row_t));

    res = sem_init(&last_row->row_lock, 1, 0);
    if(res == -1)
    {
        error = errno;
        printf("sem_init error: %s\n", strerror(error));
        return -1;
    }

    strncpy(last_row->row_name, host, NAME_SIZE);
    strncpy(last_row->row_address4, ip_v4, NAME_SIZE);
    strncpy(last_row->row_address6, ip_v6, NAME_SIZE);

    res = sem_post(&last_row->row_lock);
    if(res == -1)
    {
        error = errno;
        printf("sem_post error: %s\n", strerror(error));
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int error = 0;
    int res = 0;
    char read_buf[BUF_SIZE];
    int fd = -1;
    int i = 0;
    char *host = NULL;

    memset(read_buf, 0, BUF_SIZE);
    
    //try and open the shm, if it doesn't exist create it and initiallize its state
    fd = shm_open(SHM_NAME, O_RDWR, 0);
    if(fd == -1 && errno == ENOENT)
    {
        res = init_shm_db(&shm_addr);
        if(res == -1)
            return -1;

        hdr = (struct shm_db_hdr *)shm_addr;
        data_addr = (int *)shm_addr + sizeof(struct shm_db_hdr);
    }
    else
    {
        shm_addr = (int *)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if(shm_addr == MAP_FAILED)
        {
            error = errno;
            printf("error mapping shared memory: %s\n", strerror(error));
            return -1;
        }
        
        hdr = (struct shm_db_hdr *)shm_addr;
        data_addr = (int *)shm_addr + sizeof(struct shm_db_hdr);
    }   
    
    while(1)
    {
        //read from command line
        res = fgets(read_buf, BUF_SIZE, stdin);
        if(res == NULL)
        {
            printf("error");
            exit(-1);
        }
        else
        {
            for(i = 0; i < strlen(read_buf); i++)
            {
                if(read_buf[i] == '\n')
                {
                    read_buf[i] = '\0';
                    break;
                }
            }
        }

        //execute command
        if(strncmp(read_buf, CMD_HELP, 4) == 0)
        {
            printf("help1");
        }
        else if(strncmp(read_buf, CMD_EXIT, 4) == 0)
        {
            break;
        }
        else if(strncmp(read_buf, CMD_FETCH, 5) == 0)
        {
            host = read_buf + 6;
            res = add_entry(host);
            if(res == -1)
                break;
        }
        if(strncmp(read_buf, CMD_CHECK, 5) == 0)
        {
            printf("help1");
        }
        else if(strncmp(read_buf, CMD_SHOW, 4) == 0)
        {
            res = show_rows();
            if(res == -1)
                return -1;
        }
        else if(strncmp(read_buf, CMD_SAVE, 4) == 0)
        {
            printf("fetch1");
        }
        if(strncmp(read_buf, CMD_LOAD, 4) == 0)
        {
            printf("help1");
        }
        else if(strncmp(read_buf, CMD_CLEAR, 5) == 0)
        {
            printf("exit1");
        }
        else if(strncmp(read_buf, CMD_LOCK_TABLE, 10) == 0)
        {
            printf("fetch1");
        }
        if(strncmp(read_buf, CMD_UNLOCK_TABLE, 12) == 0)
        {
            printf("help1");
        }
        else if(strncmp(read_buf, CMD_LOCK_ROW, 8) == 0)
        {
            printf("exit1");
        }
        else if(strncmp(read_buf, CMD_UNLOCK_ROW, 10) == 0)
        {
            printf("fetch1");
        }

        memset(read_buf, 0, BUF_SIZE);
    }

    res = shm_unlink(SHM_NAME);
    if(res == -1)
    {
        error = errno;
        printf("shm_unlink error on %s: %s\n", SHM_NAME, strerror(error));
        return -1;
    }
    //do cleanup operations here

    return 0;
}
