#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http.h"
#include "net.h"

#define MAX_HOSTNAME        512
#define MAX_ARR_HANDLERS    1000

typedef struct Handler {
    char URI        [URI_SIZE]  ;
    handler_http_t  handler     ;
} Handler;

typedef struct HTTP {
    char    hostname    [MAX_HOSTNAME]      ;
    int     length                          ;
    int     max_length                      ;
    Handler arr_handlers[MAX_ARR_HANDLERS]  ;
} HTTP;

static HTTPrequest __empty_request()
{
    return (HTTPrequest){
        .METHOD = {0},
        .URI = {0},
        .VERSION = {0},
        .HOST = {0},
        .HEADERS = {0},
        .BODY = NULL,
        .__parse_stage = 0,
        .__index = 0
    };
}
static void __next_parse_stage(HTTPrequest * request)
{
    request->__parse_stage += 1;
    request->__index = 0;
}
static void __parse_request(HTTPrequest * request, char * buffer, int size)
{
    printf("%s\n", buffer);
    for(int i = 0; i < size; i++)
    {
        switch(request->__parse_stage) {
            case 0:
                if(buffer[i] == ' ' || request->__index == METHOD_SIZE-1)
                {
                    request->METHOD[request->__index] = '\0';
                    __next_parse_stage(request);
                    continue;
                }
                request->METHOD[request->__index] = buffer[i];
            break;
            case 1:
                if(buffer[i] == ' ' || request->__index == URI_SIZE-1)
                {
                    request->URI[request->__index] = '\0';
                    __next_parse_stage(request);
                    continue;
                }
                request->URI[request->__index] = buffer[i];
            break;
            case 2:
                if(buffer[i] == '\n' || request->__index == PROTOCOL_SIZE-1)
                {
                    request->VERSION[request->__index] = '\0';
                    __next_parse_stage(request);
                    continue;
                }
                request->VERSION[request->__index] = buffer[i];
            break;
            case 3:
                if(buffer[i] == ' ')
                {
                    __next_parse_stage(request);
                    continue;
                }
            break;
            case 4:
                if(buffer[i] == '\n' || request->__index == HOST_SIZE - 1)
                {
                    request->HOST[request->__index] = '\0';
                    __next_parse_stage(request);
                    continue;
                }
                request->HOST[request->__index] = buffer[i];
            break;
            case 5:
                if(((buffer[i] == '\n') && (buffer[i+2] == '\n')) || request->__index == HEADERS_SIZE - 1)
                {
                    i+=2;
                    request->HEADERS[request->__index] = '\0';
                    __next_parse_stage(request);
                    continue;
                }
                request->HEADERS[request->__index] = buffer[i];
            break;
            case 6:
                if(strlen(buffer + i) > 0)
                {
                    size_t size = strlen(buffer + i);
                    request->BODY = (char *)malloc(sizeof(char)*(size+1));
                    strcpy(request->BODY , buffer + i);
                    request->BODY[size] = '\0';
                    __next_parse_stage(request);
                }
                else
                    request->BODY = NULL;
                __next_parse_stage(request);
            break;
            default:
                return;
        }

        request->__index++;
    }

}
static int __swithch_http(HTTP * http, int connect, HTTPrequest * request)
{
    handler_http_t handler = NULL;

    /* find handler associated with URI */
    for(int i = 0; i < http->length; i++)
        if(strcmp(http->arr_handlers[i].URI, request->URI) == 0)
            handler = http->arr_handlers[i].handler;

    if(handler)
    {
        /* call first finded handker */
        handler(connect, request);
        return 0;
    }
    else
    {
        /* send default 404 reply */
        char const * header = "HTTP/1.1 404 Not Found\n\nnot found";
        int header_size = strlen(header);
        send_net(connect, header, header_size);
        return 1;
    }
}
HTTP * new_server(char const * hostname)
{
    HTTP * http = (HTTP *)malloc(sizeof(HTTP));
    strcpy(http->hostname, hostname);
    http->length = 0;
    http->max_length = MAX_ARR_HANDLERS;
    return http;
}
void free_server(HTTP * http)
{
    free(http);
}
void bind_http(HTTP * http, char const * URI, handler_http_t handler)
{
    if(http->length < http->max_length)
    {
        strcpy(http->arr_handlers[http->length].URI, URI);
        http->arr_handlers[http->length++].handler = handler;
    }
    else
    {
        fprintf(stderr, "Error: full handlers buffer for hostname=[%s]\n", http->hostname);
    }
}
int listen_http(HTTP * http)
{
    /* create tcp server */
    int listener = listen_net(http->hostname);
    if(listener < 0) return -1;

    while(1)
    {
        /* create new connection */
        int conn = accept_net(listener);
        if(conn < 0) continue;
        HTTPrequest request = __empty_request();

        /* read while not end of data */
        while(1)
        {
            char buffer[BUFSIZ] = {0};

            /* read part of data */
            int size = recv_net(conn, buffer, BUFSIZ);
            if(size < 0) break;

            /* parse part of data */
            __parse_request(&request, buffer, size);
            if(size < BUFSIZ) break;
        }

        /* find needed handler */
        __swithch_http(http, conn, &request);

        /* close connection */
        close_net(conn);
    }

    close_net(listener);
    return 0;
}
void parse_html(int connection, const char * filename, HTTPreply * reply)
{
    char status_code[4];
    char buffer[BUFSIZ+1];
    int top = 0;
    int i;

    /* packing header into buffer */
    for(i = 0; i < strlen(reply->VERSION); i++)
        buffer[top++] = reply->VERSION[i];
    buffer[top++] = ' ';
    sprintf(status_code, "%d", reply->STATUS_CODE);
    for(i = 0; i < strlen(status_code); i++)
        buffer[top++] = status_code[i];
    buffer[top++] = ' ';
    for(i = 0; i < strlen(reply->REASON); i++)
        buffer[top++] = reply->REASON[i];
    buffer[top++] = '\n';
    for(i = 0; i < strlen(reply->HEADERS); i++)
        buffer[top++] = reply->HEADERS[i];
    buffer[top++] = '\n';
    buffer[top++] = '\n';
    buffer[top] = '\0';

    /* calculate total length and send */
    size_t readSize = strlen(buffer);
    send_net(connection, buffer, readSize);

    /* send file if his passed as an argument */
    if(filename)
    {
        FILE * fp = fopen(filename, "r");
        if(fp == NULL) return;

        while((readSize = fread(buffer, sizeof(char), BUFSIZ, fp)) != 0)
            send_net(connection, buffer, readSize);

        fclose(fp);
    }
}
