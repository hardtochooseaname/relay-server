#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include <vector>
#include "Epoller.h"

class Channel;
class Server;
class Task;

class Eventloop
{
private:
    Epoller *poller_;
    std::vector<Channel*> activeChannels_ = {};
    std::vector<Task *> task_list_ = {};
    std::vector<Task *> finished_task_list_ = {};
    
    void removeFinishedTasks();

public:
    Eventloop(): poller_(new Epoller){}
    ~Eventloop(){delete poller_;}

    void loop();
    void updateChannel(Channel *channel){poller_->update(channel);}
    void addNewTask(Task *task) {task_list_.push_back(task);}
    void addFinishedTask(Task *task) {finished_task_list_.push_back(task);}
};


#endif