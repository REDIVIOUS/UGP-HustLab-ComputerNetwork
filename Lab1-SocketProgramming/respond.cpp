#include "respond.h"
#include <QDebug>
#include <sstream>
#include <string>
#include <regex>
#include <QDebug>
#include <windows.h>
#include <QTime>

//构造一个函数filetype，判断请求的文件属于何种类型
//fullpath是请求的全路径，typesuffix是文件后缀（即文件类型）
static bool filetype(const string &fullpath,const string &typesuffix)
{
    //如果文件名比后缀名还短，证明文件不存在
    if(fullpath.size()<typesuffix.size())
    {
        return false;
    }
    int full_i=fullpath.size()-1,suffix_i=typesuffix.size()-1;
    while(suffix_i>=0)
    {
        if(fullpath[full_i]!=typesuffix[suffix_i])
        {
            return false;
        }
        full_i--;
        suffix_i--;
    }
    return true;
}

RespondThread::RespondThread(QObject *parent)
    :QThread(parent)
{
//    qDebug()<<"recv thread:"<<QThread::currentThreadId();
}
RespondThread::~RespondThread()
{
    requestInterruption();
    terminate();
    quit();
    wait();
//    qDebug()<<"recv thread析构";
    closesocket(acceptSocket);
//    qDebug()<<"recv thread 析构，socket关闭";
//    qDebug()<<"recv thread running:"<<isRunning();
}

//响应模块执行函数
void RespondThread::execute()
{
    while(!isInterruptionRequested())
    {
        //参数初始化
        char recvBuf[1024],sendBuf[1024]; //接受缓冲区和发送缓冲区
        string strRecv; //接收到的报文（字符串）
        string respondhead,requesthead; //响应报文头部、请求报文头部
        string status_line,content_type,result; //构造响应报文：状态行、内容、返回结果
        string fullpath; //请求的文件的完整路径

        clientaddr.assign(inet_ntoa(clientsock.sin_addr)); //客户端地址信息字符串
        clientport=clientsock.sin_port; //客户端端口号

        //响应报文初始化
        respondhead=""; //响应报文头部清空
        result="200 OK"; //返回结果置位为 200 OK
        status_line="HTTP/1.1 200 OK\r\n"; //状态行初始化
        memset(recvBuf,0,sizeof(recvBuf)); //接受缓冲区清零

        /*请求报文的处理*/
        //接受请求报文
        int rtn; //接受的字节数
        rtn=recv(acceptsocket,recvBuf,sizeof(recvBuf),0); //按照实验手册上的设置配置recv函数
        if(rtn==SOCKET_ERROR||rtn==0) //接受失败返回；接受字节为0返回（已经断开连接）
        {
            return;
        }

        //找到请求报文头部，并用正则表达式提取URL
        strRecv.assign(recvBuf);
        size_t end=strRecv.find("\r\n\r\n");
        requesthead=strRecv.substr(0,end);
        qDebug()<<"client http header:"<<requesthead.c_str();
        regex regex_expression(R"(([A-Z]+) (.*?) HTTP/\d\.\d)"); //正则表达式
        smatch a; //正则提取数组
        if(regex_search(requesthead,a,regex_expression))
        {
            URL=a[2].str();
        }
        qDebug()<<"URL:"<<URL.c_str();


        //URL的路径换成windwos右斜杠的方式，获得全路径
        int i;
        for(i=0;i<URL.size();i++)
        {
            if(URL[i]=='/')
                URL[i]='\\';
        }
        fullpath=directorypath+URL;//完整路径

        /*处理不同类型文件*/
        //所请求的路径是一个目录，打开其下index.html
        DWORD ty;
        ty=GetFileAttributesA(fullpath.c_str());
        if((ty&FILE_ATTRIBUTE_DIRECTORY)&&(!INVALID_FILE_ATTRIBUTES))
        {
            fullpath=fullpath+"\\index.html";
        }
        //处理网页、图片、文本、类型不支持、文件不存在
        FILE* infile=fopen(fullpath.c_str(),"rb");
        if(filetype(URL,".html")||filetype(URL,"htm"))
        {
            content_type="Content-Type: text/html\r\n";
        }
        else if(filetype(URL,".txt"))
        {
            content_type="Content-Type: text/plain\r\n";
        }
        else if(filetype(URL,".jpeg")||filetype(URL,".png"))
        {
            content_type="Content-Type: image/"+URL.substr(URL.rfind('.')+1)+"\r\n";
        }
        else //类型不支持，发送501界面
        {
            fclose(infile);
            infile=fopen((directorypath+"\\501.html").c_str(),"rb");
            result="501 Not Implemented";
            firstHeader="HTTP/1.1 501 Not Inplemented\r\n";
            content_type="Content-Type: text/html\r\n";
        }
        if(!infile) //文件不存在，发送404界面
        {
            fclose(infile);
            infile=fopen((directorypath+"\\404.html").c_str(),"rb");
            result="404 Not Found";
            firstHeader="HTTP/1.1 404 Not Found\r\n";
            content_type="Content-Type: text/html\r\n";
        }

        //获取文件大小
        fseek(infile,0,SEEK_SET);
        fseek(infile,0,SEEK_END);
        flen=ftell(infile);
        fseek(infile,0,SEEK_SET);

        //构造响应报文
        respondhead=status_line
                   +content_type
                   +"Content-Length: "+to_string(flen)+"\r\n"
                   +"Server: csr_http1.1\r\n"
                   +"Connection: close\r\n"
                   +"\r\n";
        qDebug()<<"respond http header:"<<respondhead.c_str();

        /*响应过程*/
        //发送响应报文
        rtn=send(acceptsocket,respondhead.c_str(),(int)(respondhead.length()),0); //按照试验手册上进行
        //发送请求的文件
        int bufReadNum;
        while(true)
        {
            memset(sendBuf,0,sizeof(sendBuf));  //缓存清零
            bufReadNum=fread(sendBuf,1,1024,infile);
            if((send(acceptsocket,sendBuf,bufReadNum,0))==SOCKET_ERROR) //发送失败
            {
                rtn=SOCKET_ERROR;
                break;
            }
            if(feof(infile))
                break;
        }
        fclose(infile);


        /*显示ui*/
        QStringList msg; //输出的列表信息
        msg.clear();
        msg<<QString::fromStdString(clientaddr)<<QString::fromStdString(to_string(clientport))<<QString::fromStdString(requesthead);
        if(rtn==SOCKET_ERROR)
        {
            msg<<QString::fromStdString(to_string(WSAGetLastError()));
        }
        else
        {
            msg<<QString::fromStdString(result);
        }

        emit FinishCommunicate(msg);
        return;
    }
}
