#include "Server.h"
#include "Channel.h"
#include "Socket.h"
#include <iostream>
#include <algorithm>
#include "InetAddr.h"

#ifdef ISPROXY
    #include "ProxyTask.h"
#elif defined(ISECHO)
    #include "EchoTask.h"
#endif

void Server::newConnection()
{
    InetAddr addr;
    int clifd = listen_channel_->getSocket()->accept_(&addr);

#ifdef ISPROXY
    loop_->addNewTask(new ProxyTask(clifd, loop_));
#elif defined(ISECHO)
    loop_->addNewTask(new EchoTask(clifd, loop_));
#endif
    

    std::cout << "Accepted a new connection from " << addr.ip() << ":" << addr.port() << std::endl;
}

Server::Server(int port, Eventloop *loop):loop_(loop)
{
#ifdef ISPROXY
    std::cout << "Initializing Proxy Server on port " << port << "..." << std::endl;
#elif defined(ISECHO)
    std::cout << "Initializing Echo Server on port " << port << "..." << std::endl;
#endif

    listen_channel_ = new Channel(loop_, nullptr);

    InetAddr listen_addr(port);
    listen_channel_->getSocket()->bind_(&listen_addr);
    listen_channel_->getSocket()->listen_();
    
    auto cb = [this](){newConnection();};
    listen_channel_->setReadCallback(cb);
    listen_channel_->enableRead();
}

Server::~Server()
{
    if(listen_channel_)
        delete listen_channel_;
    listen_channel_ = nullptr;
}
