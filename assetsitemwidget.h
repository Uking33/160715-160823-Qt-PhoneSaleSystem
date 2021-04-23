#ifndef ASSETSITEMWIDGET_H
#define ASSETSITEMWIDGET_H

//窗口
#include <QDialog>
class AssetsWidget;
//布局
class QVBoxLayout;
class QHBoxLayout;
//控件
class UButton;
class ULine;
//数据
class AssetsData;

class AssetsItemWidget:public QDialog
{
    Q_OBJECT
public://公有函数
    AssetsItemWidget(AssetsWidget *parent, QString type);
    ~AssetsItemWidget();
    void reWidget();
    void reWidget(AssetsData * data);
public://公有控件
    QVBoxLayout *m_layout;
    //---上边布局---
    QHBoxLayout *m_upLayout;
    UButton *m_btn1;
    UButton *m_btn2;
    //---下边布局---
    QVBoxLayout *m_downLayout;
    ULine *m_name;
    ULine *m_count;
    ULine *m_price;
    ULine *m_addition;
public://公有数据
    AssetsData* m_record;
protected://事件
    void keyPressEvent(QKeyEvent *event);
private://私有函数
    void createWidget();
private://外部控件
    AssetsWidget *m_parent;
    QString m_type;
};

#endif // ASSETSITEMWIDGET_H
