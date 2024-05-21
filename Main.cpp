#include "server.hpp"

size_t handle_(char* readbuf,size_t readlen ,char** writebuf) {
    std::cout << "Request received:\n" << readbuf << std::endl;

    std::string response;
    std::string header;
    std::string body;

    // HTTPヘッダー
    header = "HTTP/1.1 200 OK\r\n";
    header += "Content-Type: text/html\r\n";
    header += "Connection: close\r\n";
    header += "\r\n";

    // HTTPボディ
    body = "<html><body><h1>Hello, World!</h1></body></html>";

    // レスポンスを結合
    response = header + body;

    // クライアントにレスポンスを送信
    *writebuf = (char*)response.c_str();
    return response.size(); 
}


//簡単な使用方法
//上のように「引数」と「戻り値」がhandle_clientと同じものを作成して
//serverのコンストラクタに登録するだけ

int main(int argc, char const *argv[])
{
    server* s = new server(8080, handle_);
    s->run_server();
    return 0;
}