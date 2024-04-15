#include "EchoTask.h"
#include "Channel.h"
#include <iostream>
#include "InetAddr.h"
#include "Socket.h"

EchoTask::EchoTask(int clifd, Eventloop *loop):Task(clifd, loop)
{
    client_channel_->getAddr()->setPeerAddr(client_channel_->getSocket());

    auto cb1 = [this](){recvData();};
    client_channel_->setReadCallback(cb1);
    auto cb2 = [this](){echoBack();};
    client_channel_->setWriteCallback(cb2);

    client_channel_->enableRead();
}

EchoTask::~EchoTask()
{}


void EchoTask::recvData()
{
    int n = client_channel_->getSocket()->recv_();
    if(n > 0){
        std::cout << "recieved " << n << " bytes" << std::endl;
        client_channel_->enableWrite();
    }
    if(n == 0){
        std::cout << "client from " << client_channel_->getAddr()->ip() << ":" << client_channel_->getAddr()->port() << " closed connection" << std::endl;
        loop_->addFinishedTask(this);
    }
}

void EchoTask::echoBack()
{
    client_channel_->getSocket()->send_();
    client_channel_->disableWrite();
}

