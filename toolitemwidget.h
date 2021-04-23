#ifndef TOOLITEMWIDGET_H
#define TOOLITEMWIDGET_H

//窗口
#include <QDialog>
class ToolWidget;
//布局
class QVBoxLayout;
class QHBoxLayout;
//控件
class UButton;
class ULine;
class UDateBox;
//数据
class ToolData;

class ToolItemWidget:public QDialog
{
    Q_OBJECT
public://公有函数
    ToolItemWidget(ToolWidget *parent, QString type);
    ~ToolItemWidget();
    void reWidget();//空白重置
    void reWidget(ToolData * data);//获取重置
public://公有数据
    QString m_type;
    ToolData* m_tool;
    ToolWidget *m_parent;
public://公有控件
    QVBoxLayout *m_layout;
    //---下边布局---
    QVBoxLayout*m_downLayout;
    ULine *m_name;
    ULine *m_count;
    ULine *m_buyingPrice;
    ULine *m_salingPrice;
    ULine *m_unit;
    //---上边布局---
    QHBoxLayout *m_upLayout;
    UButton *m_btn1;
    UButton *m_btn2;
    UButton *m_btn3;
protected://事件
    void keyPressEvent(QKeyEvent *event);
private://私有函数
    void createWidget();
private slots://私有槽函数
    void TextEdit();
};

#endif // TOOLITEMWIDGET_H
