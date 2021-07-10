
clientAndServer	:	client.o	crypto.o	server.o	tcpHandler.o	DBconnection.o	crypto.o
	g++ -pthread	-o bin/client	bin/client.o	bin/crypto.o
	g++	-pthread	-o bin/tcpServer	bin/server.o	bin/tcpHandler.o	bin/DBconnection.o	bin/crypto.o	-lmysqlcppconn	-lmysqlcppconn-static

server.o	:	src/server.cpp
	g++	-c	src/server.cpp	-o	bin/server.o

client.o	:	src/client.cpp
	g++	-c	src/client.cpp	-o	bin/client.o

tcpHandler.o	:	src/tcpHandler.cpp
	g++	-c	-pthread	src/tcpHandler.cpp	-o bin/tcpHandler.o

DBconnection.o	:	src/DBconnection.cpp
	g++	-c	src/DBconnection.cpp	-o	bin/DBconnection.o

crypto.o	:	src/crypto.cpp
	g++ -c	src/crypto.cpp	-o	bin/crypto.o

clean	:	
	rm	-f	bin/*.o
	rm	-f	bin/tcpServer
	rm	-f	bin/client
