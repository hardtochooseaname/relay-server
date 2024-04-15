#include "../include/Eventloop.h"
#include "Channel.h"
#include "Epoller.h"
#include "Task.h"

void Eventloop::loop()
{
    int n;
    while(1){
        n = poller_->epollerWait(activeChannels_);
        finished_task_list_.clear();
        for(int i = 0; i < n; i++){
            activeChannels_[i]->eventsHandler();
        }
        removeFinishedTasks(); // 每一轮完成的业务，释放其资源
    }
}

void Eventloop::removeFinishedTasks()
{
    Task *task;
    for(int i = finished_task_list_.size(); i > 0; i--){
        task = finished_task_list_.back();
        finished_task_list_.pop_back();
        task_list_.erase(std::remove(task_list_.begin(), task_list_.end(), task), task_list_.end());

        delete task;
    }
}