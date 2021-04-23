#ifndef UTIMEBOX_H
#define UTIMEBOX_H

//窗口
#include <QWidget>
class QHBoxLayout;
//控件
class QLabel;
class QLineEdit;
//数据
class QTime;

class UTimeBox:public QWidget
{
    Q_OBJECT
public:
    UTimeBox();
    ~UTimeBox();

    QString GetData();//获取时间字符串
    int GetTime(const QString type);//获取某个内容
    void Update();//更新
    void Update(QTime *time);//更新
    void Update(QString time);//更新
    QLineEdit *m_HourEdit;
    QLineEdit *m_MinEdit;
    QLineEdit *m_SecEdit;
private:
    QHBoxLayout* m_layout;
    QLabel *m_HourLabel;
    QLabel *m_MinLabel;
    QLabel *m_SecLabel;
    QLabel *m_KongLabel;

    int m_hour;
    int m_min;
    int m_sec;
private slots:
    void ChooseHour(const QString &hour);
    void ChooseMin(const QString &min);
    void ChooseSec(const QString &sec);
};

#endif // UTIMEBOX_H
