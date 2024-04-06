#include "InetAddress.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

InetAddress::InetAddress()
{

}

InetAddress::InetAddress(std::string ip, unsigned short port)
{
    bzero(&m_addr, sizeof(struct sockaddr_in));
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    m_addr.sin_addr.s_addr = ::inet_addr(ip.c_str());
}

InetAddress::~InetAddress()
{

}

const struct sockaddr* InetAddress::Addr()
{
    return (const struct sockaddr*)&m_addr;
}

std::string InetAddress::IP()
{
    return std::string(::inet_ntoa(m_addr.sin_addr));
}

int InetAddress::Port()
{
    return ntohs(m_addr.sin_port);
}

std::string InetAddress::IPPort()
{
    return IP() + ":" + std::to_string(Port());
}

void InetAddress::SetAddr(const struct sockaddr_in & addr)
{
    m_addr = addr;
}