#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <direct.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //监听地址设置
    ui->listenaddr->setText(QString("127.0.0.1"));
    //监听端口设置
    ui->listenport->setValue(8080);
    //虚拟路径的设置
    ui->path->setText(QString());
    //信息栏的设置
    ui->infoshow->setColumnCount(4); //设置列数
    ui->infoshow->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑信息栏
    QStringList list; //设置信息栏的表头
    list<<"IP"<<"Port"<<"Flie name"<<"Result"; //来源IP、端口号、HTTP请求命令行
    ui->infoshow->setHorizontalHeaderLabels(list);

    //socket编程第一步，初始化winsock环境
    if((WSAStartup(MAKEWORD(2,2),&(this->WsaData))))
    {
        qDebug()<<"winsock初始化失败:"<<WSAGetLastError();
    }
}

MainWindow::~MainWindow()
{
    closesocket(this->listensocket);
    WSACleanup();
    delete ui;
}

//按下“启动服务后的操作”
void MainWindow::BeginService()
{
    ui->start->setDisabled(true);
    //创建socket套接字
    this->listensocket=socket(AF_INET,SOCK_STREAM,0); //按照实验手册推荐的值进行设定(必须用SCOK_STREAM才能监听)
    //失败信息
    if((this->listensocket)==INVALID_SOCKET)
    {
        ui->start->setEnabled(true);
        QMessageBox::about(this,"Error","fail to init socket");   //ui弹出错误信息
        qDebug()<<"fail to init socket:"<<WSAGetLastError();  //debug弹出错误信息
        return;
    }


    //绑定socket
    sockaddr_in localaddr;
    localaddr.sin_family=AF_INET;
    localaddr.sin_port=htons(ui->listenport->value()); //绑定监听地址
    localaddr.sin_addr.S_un.S_addr=inet_addr(ui->listenaddr->text().toStdString().c_str());
    if((bind(this->listensocket,(sockaddr*)&localaddr,sizeof(localaddr)))==SOCKET_ERROR)
    {
        ui->start->setEnabled(true);
        qDebug()<<"port:"<<ui->listenport->value();
        qDebug()<<"addr:"<<inet_addr(ui->listenaddr->text().toStdString().c_str());
        qDebug()<<"in addr any:"<<htonl(INADDR_ANY);
        QMessageBox::about(this,"Error","fail to bind socket"); //ui弹出错误信息
        qDebug()<<"fail to bind socket:"<<WSAGetLastError();
        return;
    }

    //监听
    if((listen(this->listensocket,1000))==SOCKET_ERROR)
    {
        ui->start->setEnabled(true);
        QMessageBox::about(this,"Error","fail to listen");  //ui弹出错误信息
        qDebug()<<"fail to listen:"<<WSAGetLastError();  //debug弹出错误信息
        return;
    }


    //接受请求,建立新的线程
    this->newThread=new NewThread; //新建一个线程
    this->newThread->setListen(this->listensocket); //设置socket
    this->newThread->DirectoryPath(ui->path->text().toStdString());  //设置虚拟路径
    this->newThread->start();

    //接收线程rpdThread发来的信号，更新UI
    connect(this->newThread,&NewThread::getnew,[=](QStringList msg){
        qDebug()<<endl<<"msg length:"<<msg.length()<<endl;
        ui->infoshow->insertRow(ui->infoshow->rowCount());//插入一行
        QString tempStr;
        int rowIndex=ui->infoshow->rowCount();
        for(int i=0;i<msg.length();i++)
        {
            tempStr=msg.at(i);
            qDebug()<<"msg "<<i<<":"<<tempStr;
            QTableWidgetItem *item=new QTableWidgetItem(tempStr);
            ui->infoshow->setItem(rowIndex-1,i,item);
        }
    });
}

void MainWindow::CloseService()
{
    //关闭线程
    if(this->newThread!=nullptr)
    {
        delete this->newThread;
        this->newThread=nullptr;
    }
    ui->start->setEnabled(true);
    QMessageBox::about(this,"reminder","The service is closed!");
}

void MainWindow::FindDirectory()
{
    QString str=QFileDialog::getExistingDirectory(this,"选择目录",".");
    ui->path->setText(str);
}


