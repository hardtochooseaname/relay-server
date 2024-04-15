#include "../include/InetAddr.h"
#include <cstring>
#include "../include/utils.h"
#include <stdlib.h>
#include "Socket.h"

InetAddr::InetAddr()
{
    memset(&addr_, 0, sizeof(addr_));
}

InetAddr::InetAddr(unsigned short port)
{
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_.sin_port = htons(port);
}

InetAddr::InetAddr(unsigned short port, char *ipaddr)
{
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port); 
    int ret;
    ret = inet_pton(AF_INET, ipaddr, &addr_.sin_addr);
    if(ret == 0)
        perror_exit("inet_pton error: invalid ip address");
    if(ret == -1)
        perror_exit("inet_pton error: invalid address family");
}


void InetAddr::setAddr(const sockaddr_in &addr)
{
    addr_.sin_family = addr.sin_family;
    addr_.sin_addr = addr.sin_addr;
    addr_.sin_port = addr.sin_port;
}

void InetAddr::getAddr(char *ip, int &port)
{
    if(inet_ntop(AF_INET, &addr_.sin_addr, ip, 16) == nullptr)
        perror_exit("inet_ntop error");
    port = addr_.sin_port;
}

void InetAddr::setPeerAddr(Socket *sock)
{
    socklen_t len = sizeof(addr_);
    if(getpeername(sock->fd(), (sockaddr*)(&addr_), &len) < 0)  
        perror_exit("getpeername error");
}

// 成功解析返回0，失败返回-1
int InetAddr::resolveAddr4(char* address, char *&ip, int &port)
{
    int pos = 0, len = strlen(address);
    if(len <= 0)
        return -1;
    
    for(; pos < len && address[pos] != ':'; pos++);
    if(pos == len)
        return -1;

    address[pos] = 0;
    ip = address;
    port = std::atoi(address+pos+1);

    return 0;
}
