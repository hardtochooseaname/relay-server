#include "../include/Eventloop.h"
#include "Server.h"
#include <iostream>
#include <cstring>
#include "utils.h"

int main(int argc, char **argv)
{
    if(argc != 2){
#ifdef ISPROXY
        perror_exit("usage: proxyserv <port>");
#elif defined(ISECHO)
        perror_exit("usage: echoserv <port>");
#endif
    }
        
    Eventloop loop;
    Server server(std::atoi(argv[1]), &loop);
    loop.loop();

    return 0;
}

