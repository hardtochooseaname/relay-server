#include "../include/Socket.h"
#include <iostream>
#include "../include/Buffer.h"
#include <cstring>
#include "InetAddr.h"
#include <string>
#include <sstream>
#include <istream>
#include <pthread.h>
#include <vector>

#define BUFFER_SIZE 64

std::string data = "127.0.0.1:9999\n \
                    w6TrkSlhDp\n \
                    cS8YVe7x1N\n \
                    Rz7tGvJYpI\n \
                    Ndh6P9saQv\n \
                    TiOyWKvB2u\n \
                    5r7U0SVonK\n \
                    qMIWpDRL2e\n \
                    IbjlZxqvgE\n \
                    S4zFoL8k6r\n \
                    2Bu7Xw1hDQ\n \
                    xHms2RWLi0\n \
                    EJrls9jA4W\n \
                    Rb8zDTc2s1\n \
                    HmkQ3XadG5\n \
                    P5fwQmd0bL\n \
                    CNxOgQyIuj\n \
                    LJQKkAyWno\n \
                    NvRuEs7Lg2";



class Client
{
public:
    int port;
    char ip[16];
};

void clientFinished(void *args)
{
    std::cout << "thread " << pthread_self() << "exited" << std::endl;
}


/* 用户线程的启动例程
*  功能：连接到指定服务器，发送一些随机数据
*  参数：Client对象，包含目标服务的ip和端口
*  返回值：发送的数据的字节数
*/
void * newclient(void * args)
{
    unsigned long tid = (unsigned long)pthread_self();
    Client *client = (Client*)args;
    Socket sock(nullptr, BUFFER_SIZE, true, false);  // 设置成阻塞模式
    InetAddr addr(client->port, client->ip);
    sock.connect_(&addr);

    std::istringstream data_stream(data);
    std::string line;
    long count = 0;

    for(int i = 0; i < 3; i++){
        std::getline(data_stream, line);
        strncpy(sock.sendBufferPtr(), line.c_str(), BUFFER_SIZE);
        count += sock.send_();
        sock.recv_();
        printf("thread %lu received: %s\n", tid, sock.recvBufferPtr());
    }

    pthread_cleanup_push(clientFinished, 0);
    pthread_exit((void*)count);
    pthread_cleanup_pop(0);
}



int main(int argc, char** argv)
{
    if(argc != 3){
        std::cerr << "usage: client <IPaddress> <port>" << std::endl;
        exit(1);
    }

    std::vector<pthread_t> threads;
    pthread_t tid;
    Client client_;
    strncpy(client_.ip, argv[1], 16);
    client_.port = std::atoi(argv[2]);

    for(int i = 0; i < 10; i++){
        pthread_create(&tid, nullptr, newclient, (void*)&client_);
        threads.push_back(tid);
        std::cout << "created thread " << tid << std::endl;
    }

    void *n = 0;
    for(int i = 0; i < 10; i++){
        pthread_join(threads[i], &n);
        std::cout << "thread " << threads[i] << "totally sent " << (long)n << "bytes data" << std::endl;
    }

    return 0;
}