compile:	udpServer.o	client.o streamer.o
	g++ -o bin/client bin/client.o
	g++ -o bin/streamer bin/streamer.o
	g++ -pthread -o bin/udpServer bin/udpServer.o

udpServer.o:	src/udpServer.cpp
	g++ -c src/udpServer.cpp -o bin/udpServer.o

client.o:	src/client.cpp
	g++ -c src/client.cpp -o bin/client.o

streamer.o:	src/streamer.cpp
	g++ -c src/streamer.cpp -o bin/streamer.o