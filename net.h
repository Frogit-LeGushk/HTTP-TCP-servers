#ifndef H_NET
#define H_NET

#include <stddef.h>

/* server */
extern int listen_net(char const * address);
extern int accept_net(int listener);

/* client */
extern int connect_net(char const * address);

/* cut off connection */
extern int close_net(int connect);

/* both side function */
extern int send_net(int connect, char const * buffer, size_t size);
extern int recv_net(int connect, char * buffer, size_t size);

#endif // H_NET
