#include "InetAddress.h"
#include "Socket.h"
#include <iostream>

int main()
{
    Socket clientSock(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    InetAddress addr("127.0.0.1", 5001);
    clientSock.Connect(addr);

    std::string str;
    while(true)
    {
        int fd = clientSock.Fd();
        std::cin >> str;
        ::send(fd, str.c_str(), str.length(), 0);
    }

    return 0;
}