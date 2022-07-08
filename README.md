# HTTP and TCP servers:
### Screencast http server:
![alt text]()
### Screencast tcp server and client:
![alt text]()
### Description:
Wrapper for test tcp server is implemented in file `net.c` and have API in file `net.h`
TCP server and client containes in `test_net_server.c` and `test_net_client.c` and show example how use the tcp sockets API (the wrapper based on Sockets)

Wrapper for test http server is implemented in file `http.c` and have API in file `http.h` (the wrapper based on tcp wrapper)
HTTP server containes in `main.c` and show example how use the http API
