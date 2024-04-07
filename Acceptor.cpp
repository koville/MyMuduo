#include "Acceptor.h"
#include "EventLoop.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "TcpConnection.h"
#include <iostream>

Acceptor::Acceptor(EventLoop *ep, const std::string &ip, int port)
    : m_loop(ep)
{
    m_socket = new Socket();
    InetAddress serverAddr(ip, port);
    m_socket->SetReuseAddr(true);
    m_socket->SetReusePort(true);
    m_socket->SetDelay(false);
    m_socket->Bind(serverAddr);
    m_socket->Listen();

    m_loop->Ep()->AddFd(m_socket->Fd(), EPOLLIN);
    m_channel = new Channel(m_loop->Ep(), m_socket->Fd());
    m_channel->EnableRead();
    m_channel->SetReadCallback(std::bind(&Acceptor::NewConnectComming, this));
}

Acceptor::~Acceptor()
{
    if(nullptr != m_socket)
    {
        delete m_socket;
        m_socket = nullptr;
    }
    if(nullptr != m_channel)
    {
        delete m_channel;
        m_channel = nullptr;
    }
}

void Acceptor::NewConnectComming()
{
    InetAddress addr;
    int fd = m_socket->Accept(addr);
    std::cout << "New Connect comming: " << addr.IPPort() << std::endl;
    Socket *clientSock = new Socket(fd);
    TcpConnection *connection = new TcpConnection(m_loop, clientSock);
}
