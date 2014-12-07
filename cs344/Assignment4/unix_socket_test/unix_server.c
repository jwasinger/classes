#include <sys/un.h>
#include <sys/socket.h>
#include <stdarg.h>

#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100

#define BACKLOG 5

int error_exit(char *formatted_str, ...);

int main(int argc, char **argv)
{
	struct sockaddr_un addr;
	int sfd, cfd;
	ssize_t num_read;
	char buf[BUF_SIZE];
	int error;

	//create the socket
	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sfd == -1)
		error_exit("socket");

	//remove socket if it already exists on disk
	if(remove(SV_SOCK_PATH) == -1  && errno != ENOENT)
	{
		error = errno;
		error_exit("remove(): %s\n", strerror(errno));
	}

	memset(&addr, sizeof(struct sockaddr_un));
	addr.sun_familiy = AF_UNIX;
	strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

	//bind the socket so tthat it can start listening
	res = bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));
	if(res == -1)
	{
		error = errno;
		error_exit("bind(): %s\n", strerror(error));
	}

	//start listening for connections
	res = listen(sfd, BACKLOG);
	if(res == -1)
	{
		error = errno;
		error_exit("listen(): %s\n", strerror(errno));
	}

	while(1)
	{
		//accept a connection

		cfd = accept(sfd, NULL, NULL);
		if(cfd == -1)
		{
			error = errno;
			error_exit("accept(): %s\n", strerror(error));
		}

		while((num_read = read(cfd, buf, BUF_SIZE)) > 0)
			if(write(STDOUT_FILENO, buf, num_read) != num_read)
			{
				error = errno;
				error_exit("write(): %s\n", strerror(error));
			}

		if(num_read == -1)
		{
			error = errno;
			error_exit("read: %s\n", strerror(error));
		}

		if(close(cfd) == -1)
		{
			error = errno;
			error_exit("close: %s\n", strerror(error));
		}
	}

	return 0;
}
