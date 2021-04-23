#ifndef STAFFITEMWIDGET_H
#define STAFFITEMWIDGET_H

//窗口
#include <QDialog>
class StaffWidget;
//布局
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
//控件
class QLabel;
class QListWidget;
class UButton;
class ULine;
class UDateBox;
//数据
class StaffDataList;

class StaffItemWidget:public QDialog
{
    Q_OBJECT
public://公有函数
    StaffItemWidget(StaffWidget *parent, QString type);
    ~StaffItemWidget();
    void reWidget();//空白重置
    void reWidget(StaffDataList * data);//获取重置
public://公有控件
    QVBoxLayout *m_layout;
    //---上边布局---
    QHBoxLayout *m_upLayout;
    UButton *m_btn1;
    UButton *m_btn2;
    //---下边布局---
    QGridLayout *m_downLayout;
    ULine *m_ID;
    ULine *m_name;
    QHBoxLayout *m_inTime;
    QLabel *m_inTimeLabel;
    UDateBox *m_inTimeBox;
    ULine *m_addition;
    ULine *m_error;
    QHBoxLayout *m_findTimeS;
    QLabel *m_findTimeSLabel;
    UDateBox *m_findTimeSBox;
    QHBoxLayout *m_findTimeE;
    QLabel *m_findTimeELabel;
    UDateBox *m_findTimeEBox;

    ULine *m_count;
    ULine *m_price;
    ULine *m_gather;
    //项目
    QHBoxLayout *m_itemsLayout;
    QLabel *m_itemsLabel;
    QListWidget *m_itemsList;

    QHBoxLayout *m_errorsLayout;
    QLabel *m_errorsLabel;
    QListWidget *m_errorsList;
public://公有数据
    QString m_type;
public://外部数据
    StaffDataList* m_record;
    StaffWidget *m_parent;
protected:
    void keyPressEvent(QKeyEvent *event);
private://私有函数
    void createWidget();
private slots://私有槽函数
    void ButtonFindPress();
};

#endif // STAFFITEMWIDGET_H
