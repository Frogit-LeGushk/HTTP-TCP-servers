#ifdef __linux__
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#elif
    #warning "het.h: plutform not supported"
#endif


#if defined(__linux__)

#include "net.h"
#include <stdlib.h>

typedef enum error_t {
    SOCKET_ERR  = -1,
    SETOTR_ERR  = -2,
    PARSE_ERR   = -3,
    BIND_ERR    = -4,
    LISTEN_ERR  = -5,
    CONNECT_ERR = -6
} error_t;

static int __parse_adress(char const * address, char * ipv4, char * port)
{
    int i = 0;
    while(1)
    {
        if(address[i] != ':')
        {
            if(address[i] == 0) return -1;
            if(i > 14)          return -1;

            ipv4[i] = address[i];
            i++;
            continue;
        }

        if(i < 7) return -1;
        ipv4[i++] = 0;
        break;
    }

    while(address[i] != 0)
    {
        *port = address[i++];
        port++;
    }
    *port = 0;

    return 1;
}
int listen_net(char const * address)
{
    int listener = socket(AF_INET, SOCK_STREAM, 0);

    if(listener < 0)
        return SOCKET_ERR;
    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
        return SETOTR_ERR;

    char ipv4[16];
    char port[6];
    if(__parse_adress(address, ipv4, port) < 0)
        return PARSE_ERR;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipv4);
    addr.sin_port = htons(atoi(port));

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        return BIND_ERR;

    if(listen(listener, SOMAXCONN) < 0)
        return LISTEN_ERR;

    return listener;
}
int accept_net(int listener)
{
    return accept(listener, NULL, NULL);
}
int connect_net(char const * address)
{
    int conn = socket(AF_INET, SOCK_STREAM, 0);
    if(conn < 0)
        return SOCKET_ERR;

    char ipv4[16];
    char port[6];
    if(__parse_adress(address, ipv4, port) < 0)
        return PARSE_ERR;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ipv4);

    if(connect(conn, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        return CONNECT_ERR;

    return conn;
}
int close_net(int connect)
{
    return close(connect);
}
int send_net(int connect, char const * buffer, size_t size)
{
    return send(connect, buffer, size, 0);
}
int recv_net(int connect, char * buffer, size_t size)
{
    return recv(connect, buffer, size, 0);
}


#endif // defined(__linux__)
