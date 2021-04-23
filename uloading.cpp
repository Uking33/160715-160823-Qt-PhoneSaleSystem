#include "uloading.h"

#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>

#include <QLabel>

#include "data.h"
#include "type.h"
#include "uthread.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>

ULoading::ULoading(Data *data){
    //
    m_data = data;
    //界面
    progressBar = new QProgressBar();
    debug = new QLabel();
    layout = new QHBoxLayout();

    debug->setFixedWidth(120);
    debug->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    layout->addWidget(progressBar);
    layout->addWidget(debug);
    this->setLayout(layout);
}

ULoading::~ULoading(){
    delete progressBar;
    delete debug;
    delete layout;
}

void ULoading::runGet(QString str){
    if(str.contains("过程-")){
        debug->setText(str);
        m_gress++;
        progressBar->setValue(2+m_gress*97.0/m_count);
    }
    else if(str.contains("过程-配件删除")){
        debug->setText(str.left(7));
        m_gress+=str.remove(0,7).toLongLong();
        progressBar->setValue(m_gress*97.0/m_count);
        progressBar->setValue(m_gress);
    }
    else if(str.contains("连接数据库-")){
        debug->setText(str);
        m_gress=0;
        progressBar->setValue(2);
    }
    else if(str.contains("打开数据库-")){
        debug->setText(str);
        m_gress=0;
        progressBar->setValue(1);
    }
    else if(str.contains("关闭数据库-")){
        debug->setText(str);
        m_gress=100;
        progressBar->setValue(100);
        disconnect(m_data, &Data::LoadHit, this, &ULoading::runGet);
        this->close();
    }
    else if(str.contains("数量-")){
        m_count=str.remove(0,3).toLongLong();
    }
}

void ULoading::Start(QString fun, QString str1, QString str2, QString str3){
    progressBar->setValue(0);

    m_thread = new UThread;
    m_thread->Set(m_data,fun,str1,str2,str3);
    connect(m_thread, &UThread::finished, m_thread, &UThread::deleteLater);
    connect(m_data, &Data::LoadHit, this, &ULoading::runGet, Qt::QueuedConnection);
    m_thread->start();
    this->exec();
    disconnect(m_data, &Data::LoadHit, this, &ULoading::runGet);
}

void ULoading::Start(QString fun, QString str1, QString str2, QStringList strList){
    progressBar->setValue(0);

    m_thread = new UThread();
    m_thread->Set(m_data,fun,str1,str2,strList);
    connect(m_thread, &UThread::finished, m_thread, &UThread::deleteLater);
    connect(m_data, &Data::LoadHit, this, &ULoading::runGet, Qt::QueuedConnection);

    m_thread->start();
    this->exec();
    disconnect(m_data, &Data::LoadHit, this, &ULoading::runGet);
}
void ULoading::Start(QString fun, QString str1, QStringList strList1, QStringList strList2){
    progressBar->setValue(0);

    m_thread = new UThread();
    m_thread->Set(m_data,fun,str1,strList1,strList2);
    connect(m_thread, &UThread::finished, m_thread, &UThread::deleteLater);
    connect(m_data, &Data::LoadHit, this, &ULoading::runGet, Qt::QueuedConnection);

    m_thread->start();
    this->exec();
    disconnect(m_data, &Data::LoadHit, this, &ULoading::runGet);
}
void ULoading::Start(QString fun, QString str1, QString str2, QStringList strList1, QStringList strList2, PhoneData *phoneData){
    progressBar->setValue(0);

    m_thread = new UThread();
    m_thread->Set(m_data,fun,str1,str2,strList1,strList2,phoneData);
    connect(m_thread, &UThread::finished, m_thread, &UThread::deleteLater);
    connect(m_data, &Data::LoadHit, this, &ULoading::runGet, Qt::QueuedConnection);

    m_thread->start();
    this->exec();
    disconnect(m_data, &Data::LoadHit, this, &ULoading::runGet);
}

void ULoading::Start(QString fun, QString str1){
    progressBar->setValue(0);

    m_thread = new UThread();
    m_thread->Set(m_data,fun,str1);
    connect(m_thread, &UThread::finished, m_thread, &UThread::deleteLater);
    connect(m_data, &Data::LoadHit, this, &ULoading::runGet, Qt::QueuedConnection);

    m_thread->start();
    this->exec();
    disconnect(m_data, &Data::LoadHit, this, &ULoading::runGet);
}

void ULoading::Start(QString fun, QString str1, QString str2){
    progressBar->setValue(0);

    m_thread = new UThread();
    m_thread->Set(m_data,fun,str1,str2);
    connect(m_thread, &UThread::finished, m_thread, &UThread::deleteLater);
    connect(m_data, &Data::LoadHit, this, &ULoading::runGet, Qt::QueuedConnection);

    m_thread->start();
    this->exec();
    disconnect(m_data, &Data::LoadHit, this, &ULoading::runGet);
}
void ULoading::End(){
    this->close();
}

void ULoading::closeEvent(QCloseEvent *event)
{
    if(progressBar->value()<100){
        event->ignore();  //忽略退出信号，程序继续运行
        QMessageBox message(QMessageBox::Information, "提示", QString("请等待运行完毕!"));
        message.exec();
    }
}
