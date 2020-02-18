#ifndef RESPOND_H
#define RESPOND_H

#include <QObject>
#include <QThread>
#include <string>
#include <winsock2.h>
using namespace std;

class RespondThread : public QThread
{
    Q_OBJECT
public:
    explicit RespondThread(QObject *parent=0);
    ~RespondThread();
    void setlocal(sockaddr_in addr) //设置客户端地址
    {
        clientsock=addr;
    }
    void DirectoryPath(string str) //设置虚拟路径
    {
        directorypath=str;
    }
    void setaccept(SOCKET s) //设置接收到的socket
    {
        acpt=s;
    }
    void execute(); //执行函数
signals:
    void FinishCommunicate(QStringList msg);
private:
    string clientaddr; //客户地址
    int clientport; //客户端端口号
    SOCKET acceptsocket;
    sockaddr_in clientsock;
    SOCKET acpt; //接收到的socket
    string directorypath; //虚拟路径
    string request; //请求命令行
    string URL; //请求的URL
    int flen=0; //响应报文长度
    string result; //返回结果
};
#endif // RESPOND_H
