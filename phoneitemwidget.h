#ifndef PHONEITEMWIDGET_H
#define PHONEITEMWIDGET_H

//窗口
#include <QDialog>
class PhoneWidget;
//布局
class QVBoxLayout;
class QHBoxLayout;
//控件
class QLabel;
class QComboBox;
class UButton;
class UDateBox;
class UTimeBox;
class ULine;
//数据
class PhoneData;

class PhoneItemWidget:public QDialog
{
    Q_OBJECT
public://公有函数
    PhoneItemWidget(PhoneWidget *parent, const QString type, const bool edit);
    ~PhoneItemWidget();
public://公有数据
    int m_count;
public://外部数据
    bool m_model;
    QString m_type;
    PhoneWidget *m_parent;
public://公有控件
    QVBoxLayout *m_layout;
    void createWidget();
    void reWidget();
    void reWidget(PhoneData * data, bool isOut=false);
    //---左边布局---
    QVBoxLayout *m_downLayout;
    //串码IMEI
    ULine *m_IMEI;
    //手机品牌
    QHBoxLayout *m_phoneBrandLayout;
    QLabel *m_phoneBrandLabel;
    QComboBox *m_phoneBrandCombBox;
    //手机型号
    QHBoxLayout *m_phoneModelsLayout;
    QLabel *m_phoneModelsLabel;
    QComboBox *m_phoneModelsCombBox;
    //进货价
    ULine *m_buyingPrice;
    //零售价
    ULine *m_salingPrice;
    //进货时间
    QHBoxLayout *m_inTimeLayout;
    QVBoxLayout *m_inTimeDetailsLayout;
    QLabel *m_inTimeLabel;
    UDateBox *m_inTimeDateBox;
    UTimeBox *m_inTimeTimeBox;
    //备注
    ULine *m_addition;
    //---下边布局---
    QHBoxLayout *m_upLayout;
    UButton *m_btn1;
    UButton *m_btn2;
    UButton *m_btn3;
protected://事件
    void keyPressEvent(QKeyEvent *event);
private slots://私有槽函数
    void TextEdit();
};

#endif // PHONEITEMWIDGET_H
