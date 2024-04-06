#include "Epoll.h"
#include "Channel.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Epoll::Epoll()
{
    m_epollFd = epoll_create(1);
}

Epoll::~Epoll()
{
    ::close(m_epollFd);
}

void Epoll::AddFd(int fd, int event)
{
    epoll_event ev;
    bzero(&ev, sizeof(epoll_event));
    ev.data.fd = fd;
    ev.events = event;
    epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &ev);
}

void Epoll::RemoveFd(int fd)
{
    epoll_event ev;
    epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, &ev);
}

std::vector<epoll_event> Epoll::Loop(int timeout)
{
    std::vector<epoll_event> ev_vec;
    epoll_event evs[MAX_EVENT];
    int ret = epoll_wait(m_epollFd, evs, MAX_EVENT, timeout);
    if(ret == 0)
    {
        // return ev_vec;
    }
    else if(ret < 0)
    {
        perror("Epoll wait error:");
        exit(-1);
    }
    else
    {
        for(int i=0; i<ret; i++)
        {
            ev_vec.push_back(evs[i]);
        }
    }
    return ev_vec;
}

void Epoll::Update(Channel *ch)
{
    epoll_event ev;
    ev.data.ptr = ch;
    ev.events = ch->Event();
    epoll_ctl(m_epollFd, EPOLL_CTL_MOD, ch->Fd(), &ev);
}