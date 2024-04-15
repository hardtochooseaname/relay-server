#include "Task.h"
#include "Channel.h"

Task::Task(int clifd, Eventloop *loop):
    loop_(loop),
    client_channel_(new Channel(loop, &clifd, BUFFER_SIZE, true)){}


Task::~Task()
{
    delete client_channel_;
}