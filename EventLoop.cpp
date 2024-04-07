#include "EventLoop.h"
#include "Channel.h"

EventLoop::EventLoop() 
    : m_ep(new Epoll())
{

}

EventLoop::~EventLoop()
{
    if(nullptr != m_ep)
    {
        delete m_ep;
        m_ep = nullptr;
    }
}

void EventLoop::Run()
{
    char buf[1024];
    while(true)
    {   
        auto evs = m_ep->Loop();
        if(evs.size() == 0)
        {
            continue;
        }
        else
        {
            for(auto &one: evs)
            {
                ((Channel*)one.data.ptr)->SetRevent(one.events);
                ((Channel*)one.data.ptr)->HandleEvent();
            }
        }
    }
}

Epoll* EventLoop::Ep()
{
    return m_ep;
}