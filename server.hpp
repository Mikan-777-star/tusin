#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>


typedef size_t  (*handle_client)(char* readbuf,size_t readlen ,char** writebuf);

class server
{
private:
    bool henkou_flag = true;
    int PORT = 8080;
    int BUFFER_SIZE;
    handle_client hc;
    void setPORT(int port);
    int init_fd(struct sockaddr_in* address,int* addrlen);
public:
    void run_server();
    server(int port, handle_client hc);
    server(int port, int buffer_size , handle_client hc);
    ~server();
};




#endif