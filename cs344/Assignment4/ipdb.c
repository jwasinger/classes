#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#include "provided_materials/ipdb.h"

#define SHM_NAME "ipdb_shm"
#define SHM_SIZE sizeof(struct shm_db_hdr) + sizeof(ip_row_t)*MAX_ROWS
#define BUF_SIZE 256

struct shm_db_hdr 
{
	sem_t db_lock;
	int db_locked;
	int db_size;
	int num_rows;
};

int get_ip_strs(char *host, char **ip_v4, char **ip_v6);
int init_shm_db(int *addr);
int add_entry(char *host);

static struct shm_db_hdr *hdr = NULL;
static void *data_addr = NULL;
void *shm_addr = NULL;

int init_shm_db(int *addr)
{
	int fd = -1;
	int res = 0;
	int error;
	fd = shm_open(SHM_NAME, O_RDWR, 0666);
	if(fd == -1)
	{
		error = errno;
		printf("shm_open error: %s", strerror(error));
	}

	res = ftruncate(fd, SHM_SIZE);
	if(res == -1)
	{
		error = errno;
		printf("ftruncate error: %s\n", strerror(error));
		return -1;
	}
	//close(fd)?
	return 0;
}

int get_ip_strs(char *host, char **ip_v4, char **ip_v6)
{
	if(!ip_v4 || !ip_v6)
	{
		printf("invalid args\n");
		return -1;
	}

	*ip_v4 = malloc(NAME_SIZE);
	*ip_v6 = malloc(NAME_SIZE);
	return 0;
}

int add_entry(char *host)
{
	int res = 0;
	int *addr = (int *)data_addr;
	int i = 0;
	ip_row *last_row;
	int num_rows = 0;

	char ip_v4[NAME_SIZE];
	char ip_v6[NAME_SIZE];

	res = sem_wait(&hdr->db_lock);
	if(res == -1)
	{
		error = errno;
		printf("sem_wait error: %s\n", strerror(error));
		return -1;
	}

	if(hdr->db_locked)
	{
		sem_post(&hdr->db_lock);
		continue;
	}

	res = get_ip_strs(host, &ip_v4, &ip_v6);
	if(res == -1)
	{
		errno = error;
		printf("get_ip_strs error: %s\n", strerror(error));
		return -1;
	}

	num_rows = hdr->num_rows;
	hdr->num_rows++;
	sem_post(&hdr->db_lock);
	
	last_row = addr + num_rows*sizeof(ip_row);
	memset(last_row, 0, sizeof(ip_row));

	res = sem_init(last_row->row_lock, 1, 0);
	if(res == -1)
	{
		error = errno;
		printf("sem_init error: %s\n", strerror(error));
		return -1;
	}

	strncpy(last_row->row_name, host, NAME_SIZE);
	strncpy(last_row->row_address4, ipv4, NAME_SIZE);
	strncpy(last_row->row_address6, ipv6, NAME_SIZE);

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
	int fd = -1;
	int error = 0;
	int res = 0;
	char read_buf[BUF_SIZE];

	char *host = NULL;
	char ipv6[INET6_ADDRSTRLEN];
	char ipv4[INET6_ADDRSTRLEN];

	memset(read_buf, 0, BUF_SIZE);
	
	//try and open the shm, if it doesn't exist create it and initiallize its state
	res = shm_open(SHM_NAME, O_RDWR, 0);
	if(res == -1 && errno == E_NOENT)
	{
		res = init_shm_db(&shm_addr);
		if(res == -1)
			return -1;
	}
	
	addr = mmap(NULL, SHM_SIZE);
	if(addr == MAP_FAILED)
	{
		printf("error mapping shared memory\n");
		return -1;
	}

	hdr = (struct shm_db_hdr *)addr;
	data_addr = addr + sizeof(struct shm_db_hdr);

	while(1)
	{
		//read from command line
		res = fgets(read_buf, BUF_SIZE, stdin);
		if(res == NULL)
		{
			printf("error");
			exit(-1);
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
			host = read_buf + 5;
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
			printf("exit1");
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
		else if(strcmp(read_buf, CMD_LOCK_ROW, 8) == 0)
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
}