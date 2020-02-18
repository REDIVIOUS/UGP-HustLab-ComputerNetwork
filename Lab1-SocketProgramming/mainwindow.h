#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <winsock2.h>
#include "newthread.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots: //这里一定要用槽函数
    void BeginService();
    void CloseService();
    void FindDirectory();

private:
    Ui::MainWindow *ui;
    NewThread *newThread;
    SOCKET listensocket=INVALID_SOCKET;  //初始值设置为套接字没创建成功
    WSADATA WsaData;

};


#endif // MAINWINDOW_H
