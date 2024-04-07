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