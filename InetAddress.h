#pragma once 
#include <netinet/in.h>
#include <string>

class InetAddress 
{
public:
    InetAddress();
    InetAddress(std::string ip, unsigned short port);
    ~InetAddress(); 
    const struct sockaddr* Addr();
    std::string IP();
    int Port();
    std::string IPPort();
    void SetAddr(const struct sockaddr_in &addr);

private:
    struct sockaddr_in m_addr;

};