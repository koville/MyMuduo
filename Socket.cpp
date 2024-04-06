#include "Socket.h"
#include "InetAddress.h"
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#define MAX_LISTEN 128

Socket::Socket()
{
    m_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if(m_fd < 0)
    {
        perror("Create socket error:");
        exit(-1);
    }
    int opt = 1;
}

Socket::Socket(int fd)
    : m_fd(fd)
{
}

Socket::~Socket()
{
    ::close(m_fd);
    std::cout << "peer has closed:" <<m_fd << std::endl;
}

int Socket::Fd()
{
    return m_fd;
}

void Socket::SetReuseAddr(bool flag)
{
    int opt = flag ? 1 : 0;
    ::setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Socket::SetReusePort(bool flag)
{
    int opt = flag ? 1 : 0;
    ::setsockopt(m_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
}

void Socket::SetKeepAlive(bool flag)
{
    int opt = flag ? 1 : 0;
    ::setsockopt(m_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
}

void Socket::SetDelay(bool flag)
{
    int opt = flag ? 1 : 0;
    ::setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
}

void Socket::Bind(InetAddress &addr)
{
    if(0 > ::bind(m_fd, addr.Addr(), sizeof(struct sockaddr)))
    {
        perror("Bind error:");
        exit(-1);
    }
}

void Socket::Listen()
{
    if(0 > ::listen(m_fd, MAX_LISTEN))
    {
        perror("Listen error:");
        exit(-1);
    }
}

int Socket::Accept(InetAddress &addr)
{
    struct sockaddr addr_t;
    socklen_t size = sizeof(sockaddr);
    int fd = ::accept4(m_fd, &addr_t, &size, SOCK_NONBLOCK);
    if(fd < 0)
    {
        perror("Accept error:");
        exit(0);
    }
    addr.SetAddr((const struct sockaddr_in &)addr_t);
    return fd;
}

void Socket::Connect(InetAddress &addr)
{
    int ret = ::connect(m_fd, addr.Addr(), sizeof(struct sockaddr));
    if(ret < 0)
    {
        printf("----%d\n", errno);
        perror("Connect error:");
        exit(-1);
    }
}