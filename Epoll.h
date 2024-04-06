#pragma onec
#include <sys/epoll.h>
#include <vector>

class Channel;

class Epoll
{
    enum _MAX_EVENT {
        MAX_EVENT = 128
    };

public:
    Epoll();
    ~Epoll();

    void AddFd(int fd, int event);
    void RemoveFd(int fd);
    std::vector<epoll_event> Loop(int timeout = -1);
    void Update(Channel *ch);

private:
    int m_epollFd;
}; 