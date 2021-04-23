#ifndef PHONELOADING_H
#define PHONELOADING_H

//窗口
#include <QDialog>
#include <QThread>
//布局
class QHBoxLayout;
//控件
class QProgressBar;
class QPushButton;
class QLabel;
//数据
class PhoneData;
//辅助
class QTableWidget;
class PhoneThread:public QThread{
    Q_OBJECT
public:
    QList<PhoneData*> *m_dataList;
    PhoneThread(QObject *parent, QTableWidget *table, QString type, PhoneData *data, QList<PhoneData *> *IDList, QStringList strList);
protected:
    void run();
private:
    QTableWidget *m_table;
    QString m_type;
    PhoneData *m_data;
    QStringList m_strList;
private slots:
    void EDIT(int index);
    void SALE(int index);
    void SALEOUT(int index);
    void DEL(int index);
signals:
    void done();
    void LoadHit(QString str);
};

class PhoneLoading : public QDialog
{
    Q_OBJECT
public:
    PhoneLoading(QTableWidget *table);
    ~PhoneLoading();
    PhoneThread * thread;
    void Start(QString type, long long count, PhoneData *data, QList<PhoneData *> *dataList, QStringList strList);
public slots:
    void End();
protected:
    void closeEvent(QCloseEvent *event);
private:
    QProgressBar *progressBar;
    QHBoxLayout *layout;
    QLabel *debug;
    QTableWidget *m_table;
    long long m_count;
    double m_gress;
};

#endif // PHONELOADING_H
