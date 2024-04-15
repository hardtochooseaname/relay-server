#ifndef __TASK_H__
#define __TASK_H__

class Eventloop;
class Channel;

#define BUFFER_SIZE 64

class Task
{
protected:
    Eventloop *loop_ = nullptr;
    Channel *client_channel_ = nullptr;

public:
    Task(int clifd, Eventloop *loop);
    virtual ~Task();
};

#endif