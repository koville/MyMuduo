#pragma once
#include <string>

class EventLoop;
class Acceptor;

class TcpServer
{
public:
    TcpServer(const std::string &ip, int port);
    ~TcpServer();
    void Start();

private:
    EventLoop *m_loop;
    Acceptor  *m_acceptor;

};