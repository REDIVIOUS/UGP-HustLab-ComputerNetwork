#include "newthread.h"
#include <QDebug>
#include <QMessageBox>

NewThread::NewThread(QObject *parent)
    :QThread(parent)
{
    qDebug()<<"accept id: "<<QThread::currentThreadId();
}
NewThread::~NewThread()
{
    requestInterruption();
    terminate();
    quit();
    wait();
    closesocket(this->listensocket);
    emit FinishNew();
}

//创建新进程执行函数
void NewThread::run()
{
    //参数
    SOCKET snew; //建立新的socket
    sockaddr_in clientaddr; //客户端地址
    int clen=sizeof(clientaddr); //客户端地址长度

    //建立新进程
    int i=0; //目前接受的线程数量
    while(!isInterruptionRequested())
    {
        memset(&clientaddr,0,sizeof(clientaddr)); //客户地址缓存清0
        snew=accept(this->listensocket,(sockaddr*)&clientaddr,&clen); //等待新客户连接
        if(snew==INVALID_SOCKET) //如果错误返回没有成功建立线程
        {
            qDebug()<<"fail to new theard！"<<WSAGetLastError();
            return;
        }
        qDebug()<<"accept start i:"<<i; //在debug栏显示接受的线程数量
        i=i+1;

        //开启新线程
        this->newthread=new Communicate;
        this->newthread->setaccept(snew);
        this->newthread->DirectoryPath(this->directorypath);
        this->newthread->setlocal(clientaddr);
        newthread->start();
        connect(newthread,&Communicate::FinishCommunicate,this,[=](QStringList msg){
            emit getnew(msg);
        });
        connect(this,&NewThread::FinishNew,this,[=](){
            delete newthread;
            newthread=nullptr;
        });
    }
}
