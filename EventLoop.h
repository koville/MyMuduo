#pragma once 
#include "Epoll.h"

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    void Run();
    Epoll* Ep();

private:
    Epoll *m_ep;
};