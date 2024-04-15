#ifndef __ECHOTASK_H__
#define __ECHOTASK_H__

#include "Task.h"

class Eventloop;
class Channel;

class EchoTask: public Task
{
private:
    void recvData();
    void echoBack();

public:
    EchoTask(int clifd, Eventloop *loop);
    ~EchoTask();
};

#endif
