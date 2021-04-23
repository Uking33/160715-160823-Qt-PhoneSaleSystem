#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

//窗口
#include <QWidget>
class MainWindow;
class RecordItemWidget;
//布局
class QVBoxLayout;
class QHBoxLayout;
//控件
class QTableWidget;
class QLabel;
class UButton;
class UDateBox;
class UTimeBox;
class ULine;
class URectBox;
class QLineEdit;
//数据
class Data;
class PhoneRecordList;
class PhoneRecord;
//辅助
class QTableWidgetSelectionRange;

class RecordWidget : public QWidget{
    Q_OBJECT
public://公有函数
    RecordWidget(MainWindow *parent, Data *data);
    ~RecordWidget();
    void clearTable();

    void ButtonDetailsItem(QString type, QString text);//详情-详情
    void ButtonDetailsItem(QString type,PhoneRecord* record);//详情-详情
public://外部数据
    Data *m_data;
    MainWindow *m_parent;
public slots:
    void ItemDetailsFind(const QString ID);//外部强制查询
protected://事件
    void keyPressEvent(QKeyEvent *event);
private://私有函数
    void createWidget();
private slots://私有槽函数
    void ItemDetailsFindFinished(QList<PhoneRecordList *>* dataList);//外部强制查询-结束槽函数

    void ButtonFindPressFinished(QList<PhoneRecordList *> *dataList);//查询结束
    void ButtonFindPress();
    void ButtonDelPress();
    void ButtonDelAllPress();
    void ButtonDetailsPress();
    void ButtonDetailsPrint();//详情-打印
    void ButtonDetailsPrintFinished();//详情-打印结束
    void ButtonDetailsWatch();//详情-预览
    void ButtonDetailsUnSale();//详情-退货
private://私有数据
    int m_col;
    bool m_out;
    QList<PhoneRecordList *> *m_dataList;
    QStringList m_IDList;
    QList<QTableWidgetSelectionRange> m_selectionRange;    
private://私有控件
    QVBoxLayout *m_layout;
    //---表格---
    QTableWidget *m_table;
    //---按钮---
    QHBoxLayout *m_btnLayout;
    UButton *m_btn1;
    UButton *m_btn2;
    UButton *m_btn3;
    //---查询范围---
    QHBoxLayout *m_findLayout;
    //搜索
    QLabel *m_findLabel;
    QLineEdit *m_find;
    //开单时间
    QLabel *m_outdateSLabel;
    UDateBox *m_outdateSBox;
    QHBoxLayout *m_outdateSLayout;
    UButton *m_outdateSBtn1;
    UButton *m_outdateSBtn2;
    QLabel *m_outdateELabel;
    UDateBox *m_outdateEBox;
    QHBoxLayout *m_outdateELayout;
    UButton *m_outdateEBtn1;
    UButton *m_outdateEBtn2;
    URectBox *m_saleBox;//售价
    URectBox *m_gatherBox;//毛利
    QLabel *m_result;
    //---详情窗口
    RecordItemWidget *m_itemWidget;
};

#endif // RECORDWIDGET_H
