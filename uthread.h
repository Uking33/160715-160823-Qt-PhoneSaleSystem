#ifndef UTHREAD_H
#define UTHREAD_H

#include <QThread>
#include <QStringList>
class Data;
class PhoneData;

class UThread:public QThread{
    Q_OBJECT
public slots:
    void stop();
public:
    explicit UThread(QObject *parent = 0);
    void Set(Data *data, QString fun, const QString str1, QString str2, QString str3);
    void Set(Data *data, QString fun, const QString str1, QString str2, QStringList strList1);
    void Set(Data *data, QString fun, const QString str1, QStringList strList1, QStringList strList2);
    void Set(Data *data, QString fun, const QString str1, QString str2, QStringList strList1, QStringList strList2, PhoneData *phoneData);
    void Set(Data *data, QString fun, const QString str1);
    void Set(Data *data, QString fun, const QString str1, const QString str2);
protected:
    void run();
private:
    Data *m_data;
    QString m_fun;
    QString m_str1;
    QString m_str2;
    QString m_str3;
    QStringList m_strList1;
    QStringList m_strList2;
    PhoneData *m_phoneData;

    int m_model;
signals:
    void done();
    void runHint(QString str);
};

#endif // UTHREAD_H
