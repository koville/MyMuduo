#include "TcpServer.h"
#include <iostream>

int main()
{
    std::ios::sync_with_stdio(false);
    
    TcpServer server("0.0.0.0", 5001);
    server.Start();

    return 0;
}
