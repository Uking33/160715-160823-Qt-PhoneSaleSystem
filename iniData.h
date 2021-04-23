#ifndef INIDATA_H
#define INIDATA_H
#include <QObject>

class IniData:public QObject
{
    Q_OBJECT
public://公有函数
    IniData();
    ~IniData();
    void recordSave();
    void loginSave();
public://公有数据
    //账单
    QString m_recordTitle;
    QString m_recordAddress;
    QString m_recordPhone;
    QString m_recordAddition;
    //登录
    QString m_loginName;
private://私有函数
    void Loading();
};

#endif // INIDATA_H
