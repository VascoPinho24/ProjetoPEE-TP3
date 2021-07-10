
compile	:	tcpServer.o	client.o	systemCalls.o	server.o
	g++ -pthread	-o bin/client	bin/client.o
	g++	-pthread	-o bin/tcpServer	bin/server.o	bin/tcpServer.o	bin/systemCalls.o

tcpServer.o	:	src/tcpServer.cpp
	g++	-c	src/tcpServer.cpp	-o	bin/tcpServer.o

client.o	:	src/client.cpp
	g++	-c	src/client.cpp	-o	bin/client.o

server.o	:	src/server.cpp
	g++ -c src/server.cpp -o bin/server.o

systemCalls.o	:	src/systemCalls.cpp
	g++ -c src/systemCalls.cpp	-o	bin/systemCalls.o

clean	:	
	rm	-f	bin/*.o
	rm	-f	bin/tcpServer
	rm	-f	bin/client