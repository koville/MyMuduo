#pragma once
#include <string>

class EventLoop;
class Socket;
class Channel;

class Acceptor
{
public:
    Acceptor(EventLoop *ep, const std::string &ip, int port);
    ~Acceptor();

private:
    void NewConnectComming();

private:
    EventLoop *m_loop;
    Socket    *m_socket;
    Channel   *m_channel;
};