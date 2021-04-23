#ifndef ERRORITEMWIDGET_H
#define ERRORITEMWIDGET_H

//窗口
#include <QDialog>
class ErrorWidget;
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
class ErrorData;

class ErrorItemWidget:public QDialog
{
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *event);
public:
    ErrorItemWidget(ErrorWidget *parent, QString type);
    ~ErrorItemWidget();

    void createWidget();
    void reWidget();
    void reWidget(ErrorData * data);

    ErrorData* m_record;
    ErrorWidget *m_parent;
    QString m_type;
    QVBoxLayout *m_layout;
    //---上边布局---
    QGridLayout *m_upLayout;
    ULine *m_name;
    QHBoxLayout *m_date;
    QLabel *m_dateLabel;
    UDateBox *m_dateBox;
    ULine *m_price;
    ULine *m_reason;
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
    QLabel *m_hitLabel;
    QComboBox *m_hitStaffName;
    ULine *m_per;
    QPushButton *m_hitBtn1;
    QPushButton *m_hitBtn2;
    QGridLayout *m_hitLayout;
    QDialog *m_hitWidget;
    double m_max;
    long long m_staffCount;
public slots:
    void ButtonAddPress();
    void ButtonEditPress();
    void ButtonDelPress();
    void ButtonClearPress();

    void HitBoxChanged(const QString& text);
    void HitPriceChanged(const QString& text);
};


#endif // ERRORITEMWIDGET_H
