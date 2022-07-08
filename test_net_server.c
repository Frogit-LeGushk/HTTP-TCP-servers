#include "net.h"
#include <stdio.h>
#include <ctype.h>


int main()
{
    /* 1) start server */
    char const * address = "127.0.0.1:1234";
    int listener = listen_net(address);
    if(listener < 0)
    {
        fprintf(stderr, "Error listen_net: [%d]\n", listener);
        return 1;
    }

    printf("Server is listening:\t[%s]\n", address);
    int buffer_size = 256;
    char buffer[buffer_size];

    while(1)
    {
        /* 2) accept new connection with client */
        int conn = accept_net(listener);
        if(conn < 0)
        {
            fprintf(stderr, "Error accept_net:\t[%d]\n", conn);
            return 2;
        }
        printf("Create new connection:\t[%d]\n", conn);

        while(1)
        {
            /* 3) receive data from client */
            int recv_res = recv_net(conn, buffer, buffer_size);
            buffer[recv_res] = '\0';

            if(recv_res < 0)
            {
                fprintf(stderr, "Error recv_net with connection:\t[%d]\n", conn);
                break;
            }
            if(recv_res == 0)
            {
                printf("End of connection:\t[%d]\n", conn);
                break;
            }

            /* 4) processing got data ... */
            for(char * p = buffer; *p != 0; p++)
                *p = toupper(*p);

            /* 5) send processed data to client */
            int send_res = send_net(conn, buffer, recv_res);
            if(send_res < 0)
            {
                fprintf(stderr, "Error send_net with connection:\t[%d]\n", conn);
                break;
            }
        }

        int close_res = close_net(conn);
        if(close_res < 0)
        {
            fprintf(stderr, "Error close_net:\t[%d]\n", close_res);
        }
        printf("Close connection:\t[%d]\n", conn);
    }

    return 0;
}
