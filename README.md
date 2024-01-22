# HTTP and TCP servers:
### Screencast http server:
запускаем HTTP сервер, который выдает статический контент в браузере (калькулятор на js)
![alt text](https://github.com/Acool4ik/HTTP-TCP-servers/blob/master/images/screencast_http.gif)
### Screencast tcp server and client:
Пример работы сетевой библиотеки (над tcp сокетом)
![alt text](https://github.com/Acool4ik/HTTP-TCP-servers/blob/master/images/screencast_tcp.gif)
### Description:
Wrapper for test tcp server is implemented in file `net.c` and have API in file `net.h`
TCP server and client containes in `test_net_server.c` and `test_net_client.c` and show example how use the tcp sockets API (the wrapper based on Sockets)

Wrapper for test http server is implemented in file `http.c` and have API in file `http.h` (the wrapper based on tcp wrapper)
HTTP server containes in `main.c` and show example how use the http API
