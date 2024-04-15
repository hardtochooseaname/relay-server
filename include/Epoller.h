#ifndef __EPOLLER_H__
#define __EPOLLER_H__

#include <sys/epoll.h>
#include <vector>


class Channel;

class Epoller
{
public:
    Epoller();
    ~Epoller(){}

    void update(Channel * channel);
    int epollerWait(std::vector<Channel*> &activeChannels, int timeout = -1);

private:
    int epollfd_;
    std::vector<epoll_event> revents_;
};


#endif