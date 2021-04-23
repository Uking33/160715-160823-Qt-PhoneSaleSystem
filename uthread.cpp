#include "uthread.h"
#include "data.h"
#include "type.h"

UThread::UThread(QObject *parent)
        : QThread(parent)
{
}

void UThread::stop(){
    QMetaObject::invokeMethod(m_data, "Stop", Qt::DirectConnection);
}

void UThread::Set(Data *data,const QString fun, const QString str1, const QString str2, const QString str3)
{
    m_model=1;
    m_data=data;
    m_str1=str1;
    m_str2=str2;
    m_str3=str3;
    m_fun=fun;
}
void UThread::Set(Data *data, const QString fun, const QString str1, const QString str2, const QStringList strList1)
{
    m_model=2;
    m_data=data;
    m_str1=str1;
    m_str2=str2;
    m_strList1=strList1;
    m_fun=fun;
}
void UThread::Set(Data *data, QString fun, const QString str1, QStringList strList1, QStringList strList2){
    m_model=3;
    m_data=data;
    m_str1=str1;
    m_strList1=strList1;
    m_strList2=strList2;
    m_fun=fun;
}

void UThread::Set(Data *data, const QString fun, const QString str1, const QString str2, const QStringList strList1, const QStringList strList2, PhoneData *phoneData)
{
    m_model=4;
    m_data=data;
    m_str1=str1;
    m_str2=str2;
    m_strList1=strList1;
    m_strList2=strList2;
    m_phoneData=phoneData;
    m_fun=fun;
}
void UThread::Set(Data *data, QString fun, const QString str1)
{
    m_model=5;
    m_data=data;
    m_str1=str1;
    m_fun=fun;
}
void UThread::Set(Data *data, QString fun, const QString str1, const QString str2)
{
    m_model=6;
    m_data=data;
    m_str1=str1;
    m_str2=str2;
    m_fun=fun;
}
void UThread::run() {
    switch(m_model){
        case 1:
            QMetaObject::invokeMethod(m_data, m_fun.toLocal8Bit().data(), Qt::DirectConnection, Q_ARG(QString, m_str1), Q_ARG(QString, m_str2), Q_ARG(QString, m_str3));
            break;
        case 2:
            QMetaObject::invokeMethod(m_data, m_fun.toLocal8Bit().data(), Qt::DirectConnection, Q_ARG(QString, m_str1), Q_ARG(QString, m_str2), Q_ARG(QStringList, m_strList1));
            break;
        case 3:
            QMetaObject::invokeMethod(m_data, m_fun.toLocal8Bit().data(), Qt::DirectConnection, Q_ARG(QString, m_str1), Q_ARG(QStringList, m_strList1), Q_ARG(QStringList, m_strList2));
            break;
        case 4://更新
            qRegisterMetaType<PhoneData*>("PhoneData*");
            QMetaObject::invokeMethod(m_data, m_fun.toLocal8Bit().data(), Qt::DirectConnection, Q_ARG(QString, m_str1), Q_ARG(QString, m_str2), Q_ARG(QStringList, m_strList1), Q_ARG(QStringList, m_strList2), Q_ARG(PhoneData*, m_phoneData));
            break;
        case 5://获取员工
            QMetaObject::invokeMethod(m_data, m_fun.toLocal8Bit().data(), Qt::DirectConnection, Q_ARG(QString, m_str1));
            break;
        case 6://获取员工
            QMetaObject::invokeMethod(m_data, m_fun.toLocal8Bit().data(), Qt::DirectConnection, Q_ARG(QString, m_str1), Q_ARG(QString, m_str2));
            break;
    }
}
