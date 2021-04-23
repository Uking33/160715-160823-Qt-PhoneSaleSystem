#ifndef RECORDITEMWIDGET_H
#define RECORDITEMWIDGET_H

//窗口
#include <QDialog>
class RecordWidget;
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
class PhoneRecordList;

class RecordItemWidget:public QDialog
{
    Q_OBJECT
protected://事件
    void keyPressEvent(QKeyEvent *event);
public://公有函数
    RecordItemWidget(RecordWidget *parent);
    ~RecordItemWidget();
    void reWidget(PhoneRecordList * data);
public://外部数据
    RecordWidget *m_parent;
    PhoneRecordList* m_record;
private://私有函数
    void createWidget();
private://私有控件
    QVBoxLayout *m_layout;
    //---上边布局---
    QHBoxLayout *m_upLayout;
    UButton *m_btn1;
    UButton *m_btn2;
    UButton *m_btn3;
    UButton *m_btn4;
    //---下边布局---
    QGridLayout *m_downLayout;
    ULine *m_ID;
    ULine *m_outTime;
    ULine *m_shopAssistant;
    ULine *m_cashier;
    ULine *m_salePrice;
    ULine *m_gather;
    //项目
    QHBoxLayout *m_itemsLayout;
    QLabel *m_itemsLabel;
    QListWidget *m_itemsList;
};

#endif // RECORDITEMWIDGET_H
