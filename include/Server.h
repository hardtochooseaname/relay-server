#ifndef __SERVER_H__
#define __SERVER_H__

#include <vector>

class Eventloop;
class Channel;


class Server
{
private:   
    Eventloop *loop_;
    Channel *listen_channel_;

    void newConnection();

public:
    Server(int port, Eventloop *loop);
    ~Server();
};

#endif