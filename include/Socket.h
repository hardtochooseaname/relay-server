#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "Buffer.h"

class InetAddr;

class Socket: public Buffer
{
public:
    Socket(int *fdptr, int size = 0, bool one_not_two = false, bool nonblock = true); //+++ V5
    ~Socket();
    

    void bind_(const InetAddr *addr);
    void listen_(int backlog = 1024);
    int accept_(InetAddr *addr = nullptr);
    int connect_(const InetAddr *addr); // 非阻塞EINPROGRESS错误时，返回-1
    void close_();

    int fd() const {return sockfd_;}

    int send_(int n = -1) override;  //默认缓冲区有多少数据发送多少
    int recv_(int n = -1) override;  //默认能接收多少接收多少

private:
    int sockfd_;
    void setNonblock();
};

#endif