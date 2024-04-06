all: client server

client:
	g++ client.cpp InetAddress.cpp Socket.cpp -o client

server:
	g++ server.cpp InetAddress.cpp Socket.cpp Epoll.cpp Channel.cpp -o server

clean:
	rm client server
