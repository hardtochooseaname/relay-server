#include "../include/Buffer.h"
#include <unistd.h>
#include "../include/utils.h"
#include <cerrno>


Buffer::Buffer(int size, bool one_not_two)
{
    if(size){
        if(one_not_two){
            send_buffer_ = recv_buffer_ = new char[size];
        }
        else{
            send_buffer_ = new char[size];
            recv_buffer_ = new char[size];
        }
    }
    send_buffer_len_ = recv_buffer_len_ = size;
}

Buffer::~Buffer()
{
    if(send_buffer_){
        delete send_buffer_;
        if(send_buffer_ != recv_buffer_)
        delete recv_buffer_;
    }
}

