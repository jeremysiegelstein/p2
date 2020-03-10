all: server client_x client_y

server:	tcp_server.o
	g++ tcp_server.o -o server

tcp_server.o: tcp_server.cpp
	g++ -Wall -Wextra -pedantic -std=c++17 -c tcp_server.cpp

client_x:	tcp_client_x.o
	g++ tcp_client_x.o -o client_x

tcp_client_x.o: tcp_client_x.cpp
	g++ -Wall -Wextra -pedantic -std=c++17 -c tcp_client_x.cpp

client_y:	tcp_client_y.o
    g++ tcp_client_y.o -o client_y

tcp_client_y.o: tcp_client_y.cpp
    g++ -Wall -Wextra -pedantic -std=c++17 -c tcp_client_y.cpp

clean:
	-rm -f *.o server
	-rm -f *.o client_x
    -rm -f *.o client_y
