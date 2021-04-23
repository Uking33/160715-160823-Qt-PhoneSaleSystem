#ifndef ULOADING_H
#define ULOADING_H

#include <QDialog>
class QProgressBar;
class QPushButton;
class QHBoxLayout;
class QLabel;

class Data;
class PhoneData;
class UThread;

class ULoading : public QDialog
{
    Q_OBJECT
public:
    ULoading(Data *data);
    ~ULoading();
    UThread * m_thread;
public slots:
    void runGet(QString str);
    void Start(QString fun, QString str1);
    void Start(QString fun, QString str1, QString str2);
    void Start(QString fun, QString str1, QString str2, QString str3);
    void Start(QString fun, QString str1, QString str2, QStringList strList);
    void Start(QString fun, QString str1, QStringList strList1, QStringList strList2);
    void Start(QString fun, QString str1, QString str2, QStringList strList1, QStringList strList2, PhoneData *phoneData);
    void End();
protected:
    void closeEvent(QCloseEvent *event);
private:
    QProgressBar *progressBar;
    QHBoxLayout *layout;
    QLabel *debug;
    Data *m_data;
    long long m_count;
    long long m_gress;
};


#endif // ULOADING_H
