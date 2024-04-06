#pragma once
#include <sys/socket.h>
class InetAddress;

class Socket
{
public:
    Socket();
    Socket(int fd);
    ~Socket();

    int Fd();
    void SetReuseAddr(bool flag);
    void SetReusePort(bool flag);
    void SetKeepAlive(bool flag);
    void SetDelay(bool flag);

    void Bind(InetAddress &addr);
    void Listen();
    int Accept(InetAddress &addr);
    void Connect(InetAddress &addr);

private:
    int m_fd;
};
