#include <sys/un.h>
#include <sys/socket.h>
#include <starg.h>
#include <errno.h>

#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100

int main(int argc, char **argv)
{
	struct sockaddr_un addr;
	int sfd;
	ssize_t num_read;
	char buf[BUF_SIZE];

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sfd == -1)
	{
		error = errno;
		error_exit("socket() error: %s\n", strerror(error));
	}

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

	res = connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));
	if(res == -1)
	{
		error = errno;
		error_exit("connect(): %s\n", strerror(error));
	}

	while((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
		if(write(sfd, buf, num_read) != num_read)
		{
			error_exit("partial/failed writte");
		}

	if(num_read == -1)
	{
		error = errno;
		error_exit("read(): %s\n", strerror(error));
	}

	exit(EXIT_SUCCESS);
}