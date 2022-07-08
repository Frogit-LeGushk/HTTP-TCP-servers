#include "net.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>


int main()
{
    /* 1) connect to the server */
    char const * address = "127.0.0.1:1234";
    int conn = connect_net(address);
    if(conn < 0)
    {
        fprintf(stderr, "Error connect_net: [%d]\n", conn);
        return 1;
    }
    printf("Success connection to:\t[%s]\n", address);

    /* 2) read some data from keyboard */
    int buffer_size = 256;
    char buffer[buffer_size];
    printf("Enter text for sending: ");
    scanf("%[^\n]", buffer);

    printf("[%s]\n", buffer);

    /* 3) send some data to server */
    int send_res = send_net(conn, buffer, strlen(buffer));
    if(send_res < 0)
    {
        fprintf(stderr, "Error send_net:\t[%d]\n", send_res);
        return 2;
    }

    /* 4) wait response from server */
    int recv_res = recv_net(conn, buffer, buffer_size);
    buffer[recv_res] = '\0';
    if(recv_res < 0)
    {
        fprintf(stderr, "Error recv_net:\t[%d]\n", recv_res);
        return 3;
    }
    printf("Got data: %s\n", buffer);

    /* 5) close connection */
    printf("Close connection\n");
    int close_res = close_net(conn);
    if(close_res < 0)
    {
        fprintf(stderr, "Error close_net:\t[%d]\n", close_res);
        return 4;
    }

    return 0;
}

