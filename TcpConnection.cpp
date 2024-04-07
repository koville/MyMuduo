#include "TcpConnection.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"
#include <iostream>
#include "unistd.h"

TcpConnection::TcpConnection(EventLoop *ep, Socket *sock)
    : m_loop(ep)
    , m_socket(sock)
{
    m_socket->SetDelay(false);
    m_loop->Ep()->AddFd(m_socket->Fd(), EPOLLIN);
    m_channel = new Channel(m_loop->Ep(), m_socket->Fd());
    m_channel->EnableRead();
    m_channel->EnableEt();
    m_channel->SetReadCallback(std::bind(&TcpConnection::OnMessage, this));
}

TcpConnection::~TcpConnection()
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

void TcpConnection::OnMessage()
{
    const int &fd = m_socket->Fd();
    char buf[1024];
    while(true)
    {
        int ret = ::recv(fd, buf, sizeof(buf)-1, 0);
        if(ret > 0)
        {
            buf[ret] = '\0';
            std::cout << buf << std::endl;
            // ::send(fd, buf, ret, 0);
        }
        else if(ret == -1 && errno == EINTR) // 无数据情况
        {
            continue;   
        }
        else if(ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) // 全部数据接受完毕
        {
            break;
        }
        else // recognize peer has closed
        {     
            ::close(fd); // 连接已经断开 这个TcpConnection也应该被释放掉 TODO
            break;
        }
    }
}