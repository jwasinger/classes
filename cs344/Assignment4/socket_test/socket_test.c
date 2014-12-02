//from the textbook: page 1201

#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int create_socket6(int port, const char *addr)
{

}

int get_ip_from_hostname(char *host, char **output, int ip_v6)
{
    int error; 
    struct addrinfo hints, *res;
    struct in_addr addr;

    struct in6_addr addr6;
    
    if(!host || !output)
    {
        printf("bad argument 'host'\n");
        return -1;
    }

    *output = malloc(INET_ADDRSTRLEN);
    if(!*output)
    {
        error = errno;
        printf("malloc error: %s\n", strerror(error));
        return -1;
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    
    if(ip_v6 != 0)
        hints.ai_family = AF_INET6;
    else
        hints.ai_family = AF_INET;

    hints.ai_flags = 0;

    if((error = getaddrinfo(host, NULL, &hints, &res)) != 0)
    {
        if(error == EAI_NONAME)
            return 1;

        printf("getaddrinfo error: %s", gai_strerror(error));
        return -1;
    }

    if(ip_v6 != 0)
        addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
    else
        addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;

    if(ip_v6 != 0)
        inet_ntop(AF_INET6, &addr, *output, INET6_ADDRSTRLEN);
    else
        inet_ntop(AF_INET, &addr, *output, INET_ADDRSTRLEN);

    freeaddrinfo(res);

    return 0;
}

//http://pubs.opengroup.org/onlinepubs/009695399/functions/inet_ntop.html
int main(int argc, char **argv)
{
    /*char *host = "www.google.com";
    struct addrinfo hints, *res;
    struct in_addr addr;
    int err;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET6;
    hints.ai_flags = AI_V4MAPPED;

    if((err = getaddrinfo(host, NULL, &hints, &res)) != 0)
    {
        printf("getaddrinfo error: %s", strerror(err));
        return -1;
    }

    addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;

    struct addrinfo *tmp = res;
    char addr_buf[INET_ADDRSTRLEN];
    while(tmp != NULL)
    {
        addr.s_addr = ((struct sockaddr_in *)(tmp->ai_addr))->sin_addr.s_addr;
        printf("ipv6 address : %s\n", inet_ntop(AF_INET6, &addr, addr_buf, INET_ADDRSTRLEN));
        tmp = tmp->ai_next;
    }

    freeaddrinfo(res);*/
    char *ip_v6 = NULL;
    char *ip_v4 = NULL;
    int res = 0;

    res = get_ip_from_hostname("google.com", &ip_v6, 1);
    if(res != 0)
    {
        if(res != 1)
        {
            printf("error\n");
            return -1;
        }
    }
    else
    {
        printf("%s\n", ip_v6);
    }

    res = get_ip_from_hostname("google.com", &ip_v4, 0);
    if(res != 0)
    {
        if(res != 1)
        {
            printf("error\n");
            return -1;
        }
    }
    else
    {
        printf("%s\n", ip_v4);
    }

    return 0;
}
