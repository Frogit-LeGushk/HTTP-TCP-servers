#ifndef H_HTTP
#define H_HTTP

#define METHOD_SIZE             16
#define URI_SIZE                512
#define PROTOCOL_SIZE           16
#define HOST_SIZE               512
#define REASON_SIZE             32
#define HEADERS_SIZE            8192

/* anonymous structure (don't use )*/
typedef struct HTTP HTTP;
typedef struct HTTPrequest {
    /* starting line (filled by program )*/
    char METHOD         [METHOD_SIZE]           ;
    char URI            [URI_SIZE]              ;
    char VERSION        [PROTOCOL_SIZE]         ;
    char HOST           [HOST_SIZE]             ;

    /* other data (parse it yourself if u need) */
    char HEADERS        [HEADERS_SIZE]          ;
    char * BODY                                 ;

    /* parsing info (don't use) */
    unsigned char   __parse_stage               ;
    unsigned int    __index                     ;
} HTTPrequest;

typedef struct HTTPreply {
    /* starting line (fill yourself) */

    char VERSION        [PROTOCOL_SIZE]         ;
    int  STATUS_CODE                            ;
    char REASON         [REASON_SIZE]           ;

    /* other data (fill yourself) */
    char HEADERS        [HEADERS_SIZE]          ;
} HTTPreply;

/* handler interface */
typedef void (*handler_http_t)(int connection, HTTPrequest * request);

/* create/destroy server */
extern HTTP * new_server(char const * hostname);
extern void free_server(HTTP * http);

/* add handler to list of listen handlers (associate URI and handler)*/
extern void bind_http(HTTP * http, char const * URI, handler_http_t handler);

/* listen connections (main loop) */
extern int listen_http(HTTP * http);

/* reply (filename is file-data, contained into body)*/
extern void parse_html(int connection, const char * filename, HTTPreply * reply);

#endif // H_HTTP
