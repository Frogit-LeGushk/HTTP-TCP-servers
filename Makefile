CC=gcc
FLAGS=-Wall -Werror

SOURCE=main.c http.c net.c
OBJ=main.o http.o net.o

start_http_server: build run
start_tcp_server: build run_server_example
start_tcp_client: build run_client_example

build: $(SOURCE)
	$(CC) -c $(FLAGS) $(SOURCE)

run: $(OBJ)
	$(CC) $(FLAGS) $(OBJ)
	./a.out

run_server_example: test_net_server.o net.o
	$(CC) $(FLAGS) test_net_server.o net.o
	./a.out

run_client_example: test_net_client.o net.o
	$(CC) $(FLAGS) test_net_client.o net.o
	./a.out

clean:
	-rm *.o 2>/dev/null
	-rm a.out 2>/dev/null
