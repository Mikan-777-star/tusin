#include "server.hpp"
#include <cstdlib>
#include <thread>
#include <cstdio>
#include <cstdint>


bool end_flag = false;
/// @brief ポートを設定したいだけ
/// @param port 
void server::setPORT(int port)
{
    if(henkou_flag)this->PORT = port;
}


/// @brief OSからソケットをもらって初期化するまでのメゾット
/// @param address struct sockaddr_in*　（戻っても使うためポインタ acceptでいる）
/// @param addrlen int                  （これも戻り値　sockaddrの大きさが入る）
/// @return int ポート番号
int server::init_fd(struct sockaddr_in *address, int *addrlen)
{
    int opt = 1, server_fd;
    *addrlen = sizeof(struct sockaddr_in);
    //ソケットをOSからもらう
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    //ソケットに用途を設定する
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        close(server_fd);
        close(EXIT_FAILURE);
    }
    //ポートを指定して、ソケットと紐付けをする
    address->sin_family= AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(this->PORT);
    if(bind(server_fd, (struct sockaddr*) address,*addrlen) < 0){
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //待ち状態にする
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "HTTP server is running on port " << PORT << std::endl;
    return server_fd;
}

/// @brief ctrl+Cで正常終了するためのやつ
/// @param sig 
void  abrt_handler(int sig) {
  end_flag = true;
}

/// @brief これを呼び出すとctrl+Cをするまでサーバーが起動する 
void server::run_server()
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen, client_socket;
    //ポートの初期化
    server_fd = init_fd(&address, &addrlen);
    //ctrl+Cの設定（多分今後場所を変える）
    if ( signal(SIGINT, abrt_handler) == SIG_ERR ) {
        exit(1);
    }
    char* buf = new char[BUFFER_SIZE];
    char* write_buf;
    while (!end_flag)
    {
        //呼び出し待機
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        //呼び出されたら相手からの要求を読む
        int length = read(client_socket, buf, BUFFER_SIZE);
        //hc（コンストラクタに指定したメゾット）を呼び出して、リターンをもらう
        int write_len = this->hc(buf,length, &write_buf);
        //相手にレスポンスを返す
        write(client_socket, write_buf, write_len);
        //終了
        close(client_socket);
    }
    close(server_fd);
}

/// @brief コンストラクタ
/// @param port (ポート番号)
/// @param buffer_size (バッファのサイズの指定)
/// @param hc (相手の要求を読んで、レスポンスを返すためのメゾットの変数)
server::server(int port,int buffer_size, handle_client hc)
{
    BUFFER_SIZE =buffer_size;
    setPORT(port);
    this->hc = hc;
}
/// @brief ↑のバッファサイズの指定がない版　サイズは1024byteになります
/// @param port 
/// @param hc 
server::server(int port, handle_client hc){
   
    BUFFER_SIZE =1024;
    setPORT(port);
    this->hc = hc;
}

/// @brief deleteのときに呼び出されるメゾット（基本的には気にしないもの　C++の書き方だと思って）
server::~server()
{
}
