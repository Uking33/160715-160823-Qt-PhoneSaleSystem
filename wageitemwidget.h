#ifndef WAGEITEMWIDGET_H
#define WAGEITEMWIDGET_H

//窗口
#include <QDialog>
class WageWidget;
//布局
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
//控件
class UButton;
class ULine;
class UDateBox;
class QLabel;
class QListWidget;
class QComboBox;
//数据
class WageData;

class WageItemWidget:public QDialog
{
    Q_OBJECT
public://公有函数
    WageItemWidget(WageWidget *parent, QString type);
    ~WageItemWidget();
    void reWidget();
    void reWidget(WageData * data);
public://外部数据
    WageData* m_record;
    WageWidget *m_parent;
    QString m_type;
public://公有控件
    QVBoxLayout *m_layout;
    //---上边布局---
    QGridLayout *m_upLayout;
    //员工姓名
    QHBoxLayout *m_nameLayout;
    QLabel *m_nameLabel;
    QComboBox *m_nameBox;
    QHBoxLayout *m_stratDate;
    QLabel *m_stratDateLabel;
    UDateBox *m_stratDateBox;
    QHBoxLayout *m_endDate;
    QLabel *m_endDateLabel;
    UDateBox *m_endDateBox;
    ULine *m_base;
    ULine *m_gather;
    ULine *m_percent;
    QGridLayout *m_itemLayout;
    QLabel *m_itemLabel;
    QListWidget *m_itemList;
    QHBoxLayout *m_itemBtnLayout;
    UButton *m_itemBtn1;
    UButton *m_itemBtn2;
    UButton *m_itemBtn3;
    UButton *m_itemBtn4;
    //项目
    QHBoxLayout *m_itemsLayout;
    //---下边布局---
    QHBoxLayout *m_downLayout;
    UButton *m_btn1;
    UButton *m_btn2;
    //工具提示框
    ULine *m_hitName;
    ULine *m_hitPrice;
    ULine *m_hitReason;
    QPushButton *m_hitBtn1;
    QPushButton *m_hitBtn2;
    QGridLayout *m_hitLayout;
    QDialog *m_hitWidget;
    double m_max;
    long long m_staffCount;
protected:
    void keyPressEvent(QKeyEvent *event);
private://私有函数
    void createWidget();
private slots://私有槽函数
    void ButtonAddPressEnterPre();

    void ButtonAddPress();
    void ButtonEditPress();
    void ButtonDelPress();
    void ButtonClearPress();
    void ButtonDetailsPress();

    void HitNameChanged(QString);
    void HitPriceChanged(QString);
};
#endif // WAGEITEMWIDGET_H
