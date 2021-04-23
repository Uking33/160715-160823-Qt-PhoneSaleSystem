#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H
//窗口
#include <QDialog>
class MainWindow;
//布局
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
//控件
class QLabel;
class UButton;
class ULine;
class QTextEdit;
//数据
class IniData;

class SettingWidget:public QDialog
{
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void Save();
    void Watch();//预览
public:
    SettingWidget(MainWindow *parent, IniData *data);
    ~SettingWidget();

    void createWidget();
    void reWidget();

    IniData* m_data;
    MainWindow *m_parent;
    QVBoxLayout *m_layout;
    //---上边布局---
    QGridLayout *m_upLayout;
    ULine *m_recordTitle;
    ULine *m_recordAddress;
    ULine *m_recordPhone;
    QHBoxLayout *m_recordAdditionLayout;
    QLabel *m_recordAdditionLabel;
    QTextEdit *m_recordAddition;
    //---下边布局---
    QHBoxLayout *m_downLayout;
    UButton *m_btn1;
    UButton *m_btn2;
    UButton *m_btn3;
};

#endif // SETTINGWIDGET_H
