#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT_LEN 5
#define IP_LEN 50

int get_ip_port(int argc, char **argv, char *ip, char *port);

int get_ip_port(int argc, char **argv, char *ip, char *port)
{
	int c;
	int pflag;
	char *pval = NULL;
	int iflag;
	char *ival = NULL;

	while((c = getopt(argc, argv, "p:i:")))
	{
		case 'p':
			pflag = 1;
			pval = optarg;
			break;
		case 'i':
			iflag = 1;
			ival = optarg;
			break;
	}

	if(!iflag || !pflag)
	{
		printf("lacking arg(s)\n");
		return -1;
	}

	ip = ival;
	port = pval;

	return 0;
}

int main(int argc, char **argv)
{
	int res = 0;
	char ip[IP_LEN];
	char port[PORT_LEN];
	
	res = get_ip_port(argc, argv, ip, port);
	if(res == -1)
	{
		return 0;
	}
}