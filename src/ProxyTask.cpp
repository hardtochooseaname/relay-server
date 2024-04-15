#include "ProxyTask.h"
#include "Channel.h"
#include "Socket.h"
#include <cstring>
#include <iostream>
#include "InetAddr.h"


ProxyTask::ProxyTask(int client_fd, Eventloop *loop):Task(client_fd, loop)
{
    client_channel_->getAddr()->setPeerAddr(client_channel_->getSocket());

    auto cb1 = [this](){recvFromClient();};
    client_channel_->setReadCallback(cb1);
    auto cb2 = [this](){sendToClient();};
    client_channel_->setWriteCallback(cb2);
    client_channel_->enableRead();
}

ProxyTask::~ProxyTask()
{
    delete target_channel_;
    delete target_addr_;
}


void ProxyTask::recvFromClient()
{
    int n = client_channel_->getSocket()->recv_();

    // 到目标服务器的连接已经建立
    if(target_channel_){
        //std::cout << "recieved something from client" << std::endl;
        if(n > 0){
            clientToTarget();
            target_channel_->enableWrite();
        } 
        else if(n == 0){
            std::cout << "client from " << client_channel_->getAddr()->ip() << ":" << client_channel_->getAddr()->port() << " closed connection\n";
            std::cout << "close proxy connection to " << target_channel_->getAddr()->ip() << ":" << target_channel_->getAddr()->port()<< std::endl;
            loop_->addFinishedTask(this);
        }    
    }
    // 尚未建立到目标服务器的连接
    else{
        std::cout << "tring to establish connection to target server..." << std::endl;
        target_channel_ = new Channel(loop_, nullptr, BUFFER_SIZE, true);

        char *ip = nullptr;
        int port;
        InetAddr::resolveAddr4(client_channel_->getSocket()->recvBufferPtr(), ip, port);
        target_addr_ = new InetAddr(port, ip);
        
        target_channel_->getSocket()->connect_(target_addr_);
        target_channel_->getAddr()->setPeerAddr(target_channel_->getSocket());
        std::cout << "connected to target server: " << std::string(ip) << ":" << port << std::endl;
        strcpy(client_channel_->getSocket()->sendBufferPtr(), "successfully established proxy connection\n");
        client_channel_->enableWrite();

        auto cb1 = [this](){recvFromTarget();};
        target_channel_->setReadCallback(cb1);
        auto cb2 = [this](){sendToTarget();};
        target_channel_->setWriteCallback(cb2);
    }
}

void ProxyTask::sendToClient()
{
    client_channel_->getSocket()->send_();
    //std::cout << "sent something to client" << std::endl;
    client_channel_->disableWrite();
}

void ProxyTask::recvFromTarget()
{
    target_channel_->getSocket()->recv_();
    //std::cout << "received something from client" << std::endl;
    target_channel_->disableRead();
    targetToClient();
    client_channel_->enableWrite();
}

void ProxyTask::sendToTarget()
{
    target_channel_->getSocket()->send_();
    //std::cout << "sent something to target" << std::endl;
    target_channel_->disableWrite();
    target_channel_->enableRead();
}

void ProxyTask::clientToTarget()
{
    strcpy(target_channel_->getSocket()->sendBufferPtr(), client_channel_->getSocket()->recvBufferPtr());

}
void ProxyTask::targetToClient()
{
    strcpy(client_channel_->getSocket()->sendBufferPtr(), target_channel_->getSocket()->recvBufferPtr());
}


