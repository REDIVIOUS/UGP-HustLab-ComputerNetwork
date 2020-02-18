#ifndef NEWTREAD_H
#define NEWTREAD_H

#include <QObject>
#include <QThread>
#include <string>
#include <winsock2.h>
#include "communicate.h"
using namespace std;

class NewThread : public QThread
{
    Q_OBJECT
public:
    explicit NewThread(QObject *parent=0);
    ~NewThread();
    void setListen(SOCKET s) //设置监听socket
    {
        this->listensocket=s;
    }
    void DirectoryPath(string str) //设置虚拟路径
    {
        this->directorypath=str;
    }
    void run(); //执行函数
signals:
    void getnew(QStringList msg);
    void FinishNew();
private:
    SOCKET listensocket;
    string directorypath;
    Communicate *newthread;
};

#endif // NEWTREAD_H
