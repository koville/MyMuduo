#pragma once
#include <string>

class EventLoop;
class Channel;
class Socket;

class TcpConnection
{
public:
    TcpConnection(EventLoop *ep, Socket *sock);
    ~TcpConnection();

private:
    void OnMessage();

private:
    EventLoop *m_loop;
    Socket    *m_socket;
    Channel   *m_channel;

};