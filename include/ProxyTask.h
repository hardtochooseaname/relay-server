#ifndef __PROXYTASK_H__
#define __PROXYTASK_H__

#include "Task.h"

class Channel;
class InetAddr;

class ProxyTask: public Task
{
private:
    Channel *target_channel_ = nullptr;
    InetAddr *target_addr_ = nullptr;

    void clientToTarget();
    void targetToClient();

    void recvFromClient();
    void sendToClient();
    void recvFromTarget();
    void sendToTarget();

public:
    ProxyTask(int client_fd, Eventloop *loop);
    ~ProxyTask();


};

#endif
