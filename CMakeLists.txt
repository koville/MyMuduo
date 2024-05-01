cmake_minimum_required(VERSION 3.5)

# Set the project name
project (mymuduo)

set (SRCS 
    server.cpp 
    InetAddress.cpp 
    Socket.cpp 
    Epoll.cpp 
    Channel.cpp 
    TcpServer.cpp 
    Acceptor.cpp 
    EventLoop.cpp 
    TcpConnection.cpp
)


set (SRCC 
    client.cpp 
    InetAddress.cpp 
    Socket.cpp
)

add_executable(client ${SRCC})
add_executable(server ${SRCS})

# target_include_directories(server PUBLIC usr/include/c++/11)

#cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=YES ../