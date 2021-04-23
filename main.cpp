#include "mainwindow.h"
#include "loginwidget.h"
#include "data.h"
#include "iniData.h"
#include "globalapplication.h"
#include <QProcess>
#include <QFile>

int main(int argc, char *argv[])
{    
    GlobalApplication a(argc, argv);
    int ret;
    Data data;
    IniData ini;
    LogInWidget logWidget(&data,&ini,0);
    //登录
    if(logWidget.exec()==QDialog::Accepted){
        MainWindow w(&data,&ini,logWidget.GetID(),logWidget.GetName());
        if(w.m_open){
            a.setWindowInstance(&w);
            w.show();
            ret=a.exec();
        }
        else return 0;
    }
    else return 0;
    if(ret==1){
        QProcess::startDetached(qApp->applicationFilePath(),QStringList());
        return 0;
    }
    return ret;
}
