#ifndef _SERVER_HPP_
#define _SERVER_HPP_


#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <csignal>

/// @brief 変数メゾットの定義　
/// @param readbuf 相手のリクエストが入ったバッファ
/// @param readlen ↑のバッファサイズ
/// @param writebuf 相手に送るバッファの入ったポインタを返すための変数　ここに *writebuf = ... してバッファを送る
/// @return size_t ↑のバッファサイズ
typedef size_t  (*handle_client)(char* readbuf,size_t readlen ,char** writebuf);

class server
{
private:
    //疑似constのための変数　将来的に消すかも
    bool henkou_flag = true;
    //ポート番号を保存した変数
    int PORT = 8080;
    //相手のリクエストを読むためのバッファのサイズ
    int BUFFER_SIZE;
    //リクエストを読んで、レスポンスを返すためのメゾットを保存する変数
    handle_client hc;
    //Portを保存するためのメゾット
    void setPORT(int port);
    //ソケットを保存して、初期化するためのメゾット
    int init_fd(struct sockaddr_in* address,int* addrlen);
public:
    //これでサーバーが動くメゾット
    void run_server();
    //コンストラクタ
    server(int port, handle_client hc);
    server(int port, int buffer_size , handle_client hc);
    //delete
    ~server();
};




#endif