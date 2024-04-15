#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <cstring>

#define     DEFAULT_BUFFER_SIZE    128

/*
    buffer的存储结构是动态数组
    按照C风格字符串存储，要求发送和接收缓冲区需要以空字符标识结束
*/
class Buffer
{
public:
    Buffer(int size, bool one_not_two = false); 
    virtual ~Buffer();

    virtual int send_(int n = -1) = 0;  //默认缓冲区有多少数据发送多少
    virtual int recv_(int n = -1) = 0;  //默认能接收多少接收多少

    char * const recvBufferPtr(){return recv_buffer_;}
    char * const sendBufferPtr(){return send_buffer_;}

    unsigned int sendBufferSize(){return send_buffer_len_;}
    unsigned int recvBufferSize(){return recv_buffer_len_;}
    unsigned int sendDataSize(){return strlen(send_buffer_);}
    unsigned int recvDataSize(){return strlen(recv_buffer_);}

protected:
    char *send_buffer_ = nullptr;
    char *recv_buffer_ = nullptr;
    
    unsigned int send_buffer_len_;
    unsigned int recv_buffer_len_;
};


#endif