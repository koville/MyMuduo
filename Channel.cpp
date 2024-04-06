#include "Channel.h"
#include "Epoll.h"
#include "Socket.h"
#include "InetAddress.h"
#include <iostream>
#include <unistd.h>

Channel::Channel(Epoll *epoll, int fd)
    : m_ep(epoll)
    , m_fd(fd)
    , m_event(0)
    , m_revent(0)
{
    m_ep->AddFd(fd, 0);
}

Channel::~Channel()
{

}

void Channel::EnableRead()
{
    m_event |= EPOLLIN;
    m_ep->Update(this);
}

void Channel::EnableWrite()
{
    m_event |= EPOLLOUT;
    m_ep->Update(this);
}

void Channel::EnableEt()
{
    m_event |= EPOLLET;
    m_ep->Update(this);
}

void Channel::SetInEpoll(Epoll *ep)
{
    if(nullptr != m_ep)
    {
        m_ep->RemoveFd(m_fd);   
    }
    m_ep = ep;
    m_ep->AddFd(m_fd, 0);
}

bool Channel::IsInEpoll()
{
    return m_ep == nullptr;
}

void Channel::SetRevent(uint32_t revent)
{
    m_revent = revent;
}

uint32_t Channel::Event()
{
    return m_event;
}

uint32_t Channel::REvent()
{
    return m_revent;
}

int Channel::Fd()
{
    return m_fd;
}

void Channel::SetReadCallback(callback_ cb)
{
    m_readCallback = cb;
}

void Channel::SetCloseCallback(callback_ cb)
{
    m_closeCallback = cb;
}

void Channel::HandleEvent()
{
    if(m_revent & EPOLLIN)
    {
        m_readCallback();
    }
    else if(m_revent & EPOLLHUP)
    {
        m_closeCallback();
    }
    else
    {

    }
}

void Channel::OnNewConnectComming(Socket *sock)
{
    InetAddress addr;
    int fd = sock->Accept(addr);
    std::cout << "New Connect comming: " << addr.IPPort() << std::endl;
    Channel *ch = new Channel(m_ep, fd);
    ch->EnableRead();
    ch->EnableEt();
    Socket *clientSock = new Socket(fd);
    ch->SetReadCallback(std::bind(&Channel::OnMessage, ch, clientSock));
    ch->SetCloseCallback(std::bind(&Channel::OnClose, ch, clientSock));
}

void Channel::OnMessage(Socket *sock)
{
    const int &fd = sock->Fd();
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
            std::cout << "111111" << std::endl;
            continue;   
        }
        else if(ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) // all data has been revc
        {
            std::cout << "22222" << std::endl;
            break;
        }
        else // recognize peer has closed
        {     
            std::cout << "3333" << std::endl;
            std::invoke(m_closeCallback);
            break;
        }
    }

}

void Channel::OnClose(Socket *sock)
{
    if(nullptr != sock)
    {
        delete sock;
    }
}