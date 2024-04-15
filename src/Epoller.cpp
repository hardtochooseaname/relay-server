#include "../include/Epoller.h"
#include "../include/utils.h"
#include "../include/Socket.h"
#include <cerrno>
#include "../include/Channel.h"

Epoller::Epoller():epollfd_(epoll_create1(0))
{
    if(epollfd_ < 0)
        perror_exit("epoll_create1 error");
    revents_.resize(16);
}


//--- V5
// void Epoller::registry(Channel *channel)
// {
//     if(channel->isInEpoller())
//         update(channel);
//     else{
//         int ret = epoll_ctl(epollfd_, EPOLL_CTL_ADD, channel->getSocket()->fd(), channel->event());
//         if(ret < 0)
//             perror_exit("registry error");
//     }
// }

//++-- V5
void Epoller::update(Channel *channel)
{
    if(channel->isInEpoller()){
        int ret = epoll_ctl(epollfd_, EPOLL_CTL_MOD, channel->getSocket()->fd(), channel->event());
        if(ret < 0)
            perror_exit("update error: MOD");        
    }
    else{
        int ret = epoll_ctl(epollfd_, EPOLL_CTL_ADD, channel->getSocket()->fd(), channel->event());
        if(ret < 0)
            perror_exit("update error: ADD");    
        channel->registered_ = true;  
    }
}



int Epoller::epollerWait(std::vector<Channel*> &activeChannels, int timeout)
{
    int n = epoll_wait(epollfd_, &revents_[0], revents_.size(), timeout);

    if(n < 0){
        if(errno == EINTR)  // EINTR表示epoll_wait正常返回前被信号打断
            return n;
        perror_exit("epollwait error");
    }
        
    activeChannels.clear();
    Channel *channel;
    for(int i = 0; i < n; i++){
        channel = static_cast<Channel*>(revents_[i].data.ptr);
        channel->setRevents(revents_[i].events);
        activeChannels.push_back(channel);
    }

    if(n == revents_.size())
        revents_.resize(revents_.size() * 2);

    return n;
}