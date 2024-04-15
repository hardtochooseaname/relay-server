#include "../include/Socket.h"
#include "../include/utils.h"
#include <cerrno>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include "InetAddr.h"

Socket::Socket(int *fdptr, int size, bool one_not_two, bool nonblock):Buffer(size, one_not_two)
{
    if(fdptr == nullptr){
        if((sockfd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            perror_exit("socket error");
    }
    else
        sockfd_ = *fdptr;

    if(nonblock)
        setNonblock();
}


Socket::~Socket()
{
    close_();
}

void Socket::setNonblock()
{
    int flags = fcntl(sockfd_, F_GETFL, 0);
    if(fcntl(sockfd_, F_SETFL, flags|O_NONBLOCK) < 0)
        perror_exit("fcntl error");  
}


void Socket::bind_(const InetAddr *addr)
{
    if(bind(sockfd_, reinterpret_cast<const sockaddr*>(addr->getAddrPtr()), sizeof(*addr->getAddrPtr())) < 0)
        perror_exit("bind error");
}

void Socket::listen_(int backlog)
{
    if(listen(sockfd_, backlog) < 0)
        perror_exit("listen error");
}

int Socket::accept_(InetAddr *addr)
{
    int fd;
    socklen_t len;

again:
    if(addr){
        sockaddr_in cliaddr;
        memset(&cliaddr, 0, sizeof(cliaddr));
        len = sizeof(cliaddr);  // 不关心accept返回的address length
        fd = accept(sockfd_, reinterpret_cast<sockaddr*>(&cliaddr), &len); 
        if(fd >= 0)
            addr->setAddr(cliaddr);
    }
    else   
        fd = accept(sockfd_, nullptr, nullptr);

    if(fd < 0){
        if(errno == EPROTO || errno == ECONNABORTED)   // 连接没有成功建立（可容忍的错误）
            goto again;
        if(errno != EAGAIN && errno != EWOULDBLOCK) 
            perror_exit("accpet error");        
        // 非阻塞模式直接返回-1
    }

    return fd;
}

int Socket::connect_(const InetAddr *addr)
{
    int n = connect(sockfd_, 
                    reinterpret_cast<const sockaddr*>(addr->getAddrPtr()), 
                    sizeof(*addr->getAddrPtr()));
    if(n < 0 && errno != EINPROGRESS)
        perror_exit("connect error");

    return n;
}

void Socket::close_()
{
    while(close(sockfd_) < 0){
        if(errno != EINTR)
            perror_exit("close error");
        std::cerr << "close() EINTR error" << std::endl;
    }
    //std::cout << "connection close: fd " << sockfd_ << std::endl;
}

int Socket::send_(int n)
{
    int ret;
    int datasize = sendDataSize();

    if(n == -1 || n >= datasize)
        ret = write(sockfd_, send_buffer_, datasize);
    else if(n > 0 && n < datasize)
        ret = write(sockfd_, send_buffer_, n);
    else
        perror_exit("Buffer::send_(int n) error: invalid argument n");

    if(ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        perror_exit("write error");
    return ret;
}

// 阻塞模式下：如果read出错直接exit，返回值一定>=0
// 非阻塞模式下：只有在数据不可读时返回-1，其他错误同样exit
int Socket::recv_(int n)
{
    int ret;

    if(n == -1 || n >= recv_buffer_len_)
        ret = read(sockfd_, recv_buffer_, recv_buffer_len_-1);//-1 留空位给空字符
    else if(n > 0 && n < recv_buffer_len_)
        ret = read(sockfd_, recv_buffer_, n);
    else
        perror_exit("Buffer::recv_(int n) error: invalid argument n");

    if(ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        perror_exit("read error");
    else if(ret > 0)
        recv_buffer_[ret] = 0; // 添加空字符
    else if(ret == 0)
        recv_buffer_[0] = 0;    // 接收到EOF
    return ret;   
}