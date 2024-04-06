#include "InetAddress.h"
#include "Socket.h"
#include "Epoll.h"
#include "Channel.h"
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

int main()
{
    std::ios::sync_with_stdio(false);
    Socket serverSock;
    InetAddress serverAddr("0.0.0.0", 5001);
    serverSock.SetReuseAddr(true);
    serverSock.SetReusePort(true);
    serverSock.Bind(serverAddr);
    serverSock.Listen();

    Epoll ep;
    ep.AddFd(serverSock.Fd(), EPOLLIN);
    Channel ch(&ep, serverSock.Fd());
    ch.EnableRead();
    ch.SetReadCallback(std::bind(&Channel::OnNewConnectComming, &ch, &serverSock));
    ch.SetCloseCallback(std::bind(&Channel::OnClose, &ch, &serverSock));

    char buf[1024];
    while(true)
    {   
        auto evs = ep.Loop();
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

    return 0;
}
