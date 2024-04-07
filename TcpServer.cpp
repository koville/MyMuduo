#pragma once
#include "TcpServer.h"
#include "EventLoop.h"
#include "Acceptor.h"

TcpServer::TcpServer(const std::string &ip, int port)
{
    m_loop = new EventLoop();
    m_acceptor = new Acceptor(m_loop, ip, port);
}

TcpServer::~TcpServer()
{
    if(nullptr != m_loop)
    {
        delete m_loop;
        m_loop = nullptr;
    }
    if(nullptr != m_acceptor)
    {
        delete m_acceptor;
        m_acceptor = nullptr;
    }
}

void TcpServer::Start()
{
    m_loop->Run();
}