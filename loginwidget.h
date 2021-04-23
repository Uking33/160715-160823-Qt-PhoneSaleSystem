#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

//窗口
#include <QDialog>
//布局
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
//控件
class QLabel;
class QListWidget;
class UButton;
class ULine;
//数据
class Data;
class IniData;
class PhoneRecordList;

class LogInWidget:public QDialog
{
    Q_OBJECT
public slots:
    QString GetID();
    QString GetPassword();
    QString GetName();
private slots:
    void Login();
protected:
    void keyPressEvent(QKeyEvent *event);
public:
    LogInWidget(Data *data,IniData *ini, short type=0, QString ID="", QString lastPassword="");
    ~LogInWidget();

    void createWidget(QString ID);

    short m_type;
    QString m_lastPassword;
    Data *m_data;
    IniData *m_ini;
    QVBoxLayout *m_layout;
    //---上边布局---
    QGridLayout *m_upLayout;
    ULine *m_ID;
    ULine *m_password;
    //---下边布局---
    QHBoxLayout *m_downLayout;
    UButton *m_btn1;
    UButton *m_btn2;
};


#endif // LOGINWIDGET_H
