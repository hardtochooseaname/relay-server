#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <sys/epoll.h>
#include <functional>
#include "Eventloop.h"

//std::function是一个通用的函数封装类，
//可以容纳任何可以调用的目标——函数、函数指针、函数对象、成员函数指针等
using EventCallback = std::function<void()>;

class Socket ;
class InetAddr;

class Channel
{
public:
    Channel(Eventloop *loop, int *fdptr, int size = 0, int one_not_two = false, bool nonblock = true);
    ~Channel();

    void setEvents(unsigned int events);
    unsigned int getEvents() const {return events_;}
    void setRevents(unsigned int revents){revents_ = revents;}
    unsigned int getRevents() const {return revents_;}
    epoll_event *event() {return &event_;}
    //epoll_event const *event() const {return &event_;}

    void setReadCallback(EventCallback callback){readCallback_ = callback;}
    void setWriteCallback(EventCallback callback){writeCallback_ = callback;}
    void seterrorCallback(EventCallback callback){errorCallback_ = callback;}
    void setdisconnectCallback(EventCallback callback){disconnectCallback_ = callback;}    
    void eventsHandler();

    void enableRead(){setEvents(events_|EPOLLIN); loop_->updateChannel(this);}
    void disableRead(){setEvents(events_&~EPOLLIN); loop_->updateChannel(this);}
    void enableWrite(){setEvents(events_|EPOLLOUT); loop_->updateChannel(this);}
    void disableWrite(){setEvents(events_&~EPOLLOUT); loop_->updateChannel(this);}

    bool isError(){return revents_ & EPOLLERR;}
    bool isDisconnected(){return revents_ & EPOLLHUP;}
    bool isReadful(){return revents_ & EPOLLIN;}
    bool isWriteful(){return revents_ & EPOLLOUT;}

    bool isInEpoller() const {return registered_;}
    bool registered_ = false;   // 标记channel是否注册到了epoller中
    Socket *getSocket() const {return sock_;}
    InetAddr *getAddr() const {return addr_;}

private:
    Eventloop *loop_;
    Socket *sock_;
    InetAddr *addr_;

    epoll_event event_;    // 单数event_ 是epoll_event结构体
    unsigned int events_ = 0;  // 复数events_ 是关心的事件
    unsigned int revents_ = 0;  // 某一轮epoll_wait返回的就绪事件

    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
    EventCallback disconnectCallback_;
};


#endif