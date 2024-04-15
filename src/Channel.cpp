#include "../include/Channel.h"
#include "../include/Socket.h"
#include "InetAddr.h"

Channel::Channel(Eventloop *loop, int *fdptr, int size, int one_not_two, bool nonblock)
    :loop_(loop), 
    addr_(new InetAddr()),
    sock_(new Socket(fdptr, size, one_not_two, nonblock)){}

Channel::~Channel()
{
    delete sock_;
    delete addr_;
}

void Channel::setEvents(unsigned int events)
{
    event_.data.ptr = this;
    event_.events = events_ = events;
}

void Channel::eventsHandler()
{
    if(isError())  // 出错直接exit，所以最先判断
        errorCallback_();    
    if(isReadful())
        readCallback_();
    if(isWriteful())
        writeCallback_();
    if(isDisconnected())
        disconnectCallback_();
}