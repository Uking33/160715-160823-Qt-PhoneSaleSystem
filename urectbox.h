#ifndef URECTBOX_H
#define URECTBOX_H

//窗口
#include <QWidget>
class QHBoxLayout;
//控件
class QLabel;
class QLineEdit;
class QCheckBox;

class URectBox:public QWidget
{
    Q_OBJECT
public:
    URectBox(const QString & title,const QString & from,const QString & to);
    ~URectBox();

    long long GetFrom();//获取from
    long long GetTo();//获取to
    void Update();//刷新更新    
    void SetShortcut(QString key);//设置快捷键

    QCheckBox *m_checkBox;
private:
    QHBoxLayout* m_layout;
    QLabel *m_titleLabel;
    QLabel *m_fromLabel;
    QLabel *m_toLabel;
    QLineEdit *m_fromEdit;
    QLineEdit *m_toEdit;

    long long m_from;
    long long m_to;
private slots:
    void stateChanged(int state);
};

#endif // URECTBOX_H
