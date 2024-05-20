#include "server.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <cstdio>
#include <cstdint>


bool end_flag = false;

void server::setPORT(int port)
{
    if(henkou_flag)this->PORT = port;
}

int server::init_fd(struct sockaddr_in *address, int *addrlen)
{
    int opt = 1, server_fd;
    *addrlen = sizeof(struct sockaddr_in);
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        close(server_fd);
        close(EXIT_FAILURE);
    }
    address->sin_family= AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(this->PORT);
    if(bind(server_fd, (struct sockaddr*) address,*addrlen) < 0){
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "HTTP server is running on port " << PORT << std::endl;
    return server_fd;
}

void  abrt_handler(int sig) {
  end_flag = true;
}


void server::run_server()
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen, client_socket;
    server_fd = init_fd(&address, &addrlen);
    if ( signal(SIGINT, abrt_handler) == SIG_ERR ) {
        exit(1);
    }
    char* buf = new char[BUFFER_SIZE];
    char* write_buf;
    while (!end_flag)
    {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        int length = read(client_socket, buf, BUFFER_SIZE);
        int write_len = this->hc(buf,length, &write_buf);
        write(client_socket, write_buf, write_len);
        close(client_socket);
    }
    close(server_fd);
}

server::server(int port,int buffer_size, handle_client hc)
{
    BUFFER_SIZE =buffer_size;
    setPORT(port);
    this->hc = hc;
}
server::server(int port, handle_client hc){
   
    BUFFER_SIZE =1024;
    setPORT(port);
    this->hc = hc;
}

server::~server()
{
}
