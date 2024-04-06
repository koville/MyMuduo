#pragma once 
#include <bits/stdint-uintn.h>
#include <functional>

using callback_ = std::function<void()>;

class Epoll;
class Socket;

class Channel
{
public:
    Channel(Epoll *epoll, int fd);
    ~Channel();
    void EnableRead();
    void EnableWrite();
    void EnableEt();
    void SetInEpoll(Epoll *ep);
    bool IsInEpoll();
    void SetRevent(uint32_t rrvent);
    uint32_t Event();
    uint32_t REvent();
    int      Fd();
    void SetReadCallback(callback_ cb);
    void SetCloseCallback(callback_ cb);

    void HandleEvent();

    void OnNewConnectComming(Socket *sock);
    void OnMessage(Socket *sock);
    void OnClose(Socket *sock);

private:
    Epoll    *m_ep;
    int       m_fd;
    uint32_t  m_event;
    uint32_t  m_revent;
    callback_ m_readCallback;
    callback_ m_closeCallback;
};