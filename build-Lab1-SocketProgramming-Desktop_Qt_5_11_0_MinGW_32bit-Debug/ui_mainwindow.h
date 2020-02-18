/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *path;
    QLabel *addrLabel;
    QSpinBox *listenport;
    QPushButton *start;
    QTableWidget *infoshow;
    QLabel *portLabel;
    QLineEdit *listenaddr;
    QLabel *pathLabel;
    QPushButton *close;
    QPushButton *finddirectory;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(866, 883);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        path = new QLineEdit(centralWidget);
        path->setObjectName(QStringLiteral("path"));
        path->setGeometry(QRect(80, 80, 301, 31));
        addrLabel = new QLabel(centralWidget);
        addrLabel->setObjectName(QStringLiteral("addrLabel"));
        addrLabel->setGeometry(QRect(20, 20, 121, 51));
        listenport = new QSpinBox(centralWidget);
        listenport->setObjectName(QStringLiteral("listenport"));
        listenport->setGeometry(QRect(300, 30, 101, 31));
        listenport->setMinimum(-1);
        listenport->setMaximum(65535);
        listenport->setValue(0);
        start = new QPushButton(centralWidget);
        start->setObjectName(QStringLiteral("start"));
        start->setGeometry(QRect(120, 130, 81, 31));
        infoshow = new QTableWidget(centralWidget);
        infoshow->setObjectName(QStringLiteral("infoshow"));
        infoshow->setGeometry(QRect(40, 180, 401, 291));
        portLabel = new QLabel(centralWidget);
        portLabel->setObjectName(QStringLiteral("portLabel"));
        portLabel->setGeometry(QRect(240, 20, 121, 51));
        listenaddr = new QLineEdit(centralWidget);
        listenaddr->setObjectName(QStringLiteral("listenaddr"));
        listenaddr->setGeometry(QRect(80, 30, 101, 31));
        pathLabel = new QLabel(centralWidget);
        pathLabel->setObjectName(QStringLiteral("pathLabel"));
        pathLabel->setGeometry(QRect(20, 70, 121, 51));
        close = new QPushButton(centralWidget);
        close->setObjectName(QStringLiteral("close"));
        close->setGeometry(QRect(250, 130, 81, 31));
        finddirectory = new QPushButton(centralWidget);
        finddirectory->setObjectName(QStringLiteral("finddirectory"));
        finddirectory->setGeometry(QRect(380, 80, 25, 31));
        finddirectory->setMinimumSize(QSize(25, 0));
        finddirectory->setMaximumSize(QSize(25, 16777215));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 866, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(finddirectory, SIGNAL(clicked()), MainWindow, SLOT(FindDirectory()));
        QObject::connect(close, SIGNAL(clicked()), MainWindow, SLOT(CloseService()));
        QObject::connect(start, SIGNAL(clicked()), MainWindow, SLOT(BeginService()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        addrLabel->setText(QApplication::translate("MainWindow", "\347\233\221\345\220\254\345\234\260\345\235\200", nullptr));
        start->setText(QApplication::translate("MainWindow", "\345\220\257\345\212\250\346\234\215\345\212\241", nullptr));
        portLabel->setText(QApplication::translate("MainWindow", "\347\233\221\345\220\254\347\253\257\345\217\243", nullptr));
        pathLabel->setText(QApplication::translate("MainWindow", "\347\233\256\345\275\225\350\267\257\345\276\204", nullptr));
        close->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\346\234\215\345\212\241", nullptr));
        finddirectory->setText(QApplication::translate("MainWindow", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
