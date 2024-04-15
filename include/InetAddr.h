#ifndef __INETADDR_H__
#define __INETADDR_H__

#include <arpa/inet.h>
#include <string>

class Socket;

class InetAddr
{
public:
    InetAddr();
    InetAddr(unsigned short port);
    InetAddr(unsigned short port, char * ipaddr);
    InetAddr(const sockaddr_in &addr):addr_(addr){}

    const sockaddr_in* getAddrPtr() const {return &addr_;}
    void setAddr(const sockaddr_in &addr);
    void getAddr(char *ip, int &port);
    void setPeerAddr(Socket *sock);
    std::string ip(){char ip[16]; inet_ntop(AF_INET, &addr_.sin_addr, ip, 16); return ip;}
    int port(){return ntohs(addr_.sin_port);}

    static int resolveAddr4(char* address, char *&ip, int &port);   // 解析形如"127.0.0.1:80"的地址结构

private:
    sockaddr_in addr_;

};


#endif