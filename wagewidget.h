#ifndef WAGEWIDGET_H
#define WAGEWIDGET_H

//窗口
#include <QWidget>
class MainWindow;
class WageItemWidget;
//布局
class QVBoxLayout;
class QHBoxLayout;
//控件
class QTableWidget;
class QLabel;
class QLineEdit;
class UButton;
class ULine;
class UDateBox;
class URectBox;
//数据
class Data;
class WageData;
class StaffDataList;
//辅助
class QTableWidgetSelectionRange;

class WageWidget : public QWidget{
    Q_OBJECT
public://公有函数
    WageWidget(MainWindow *parent, Data *data);
    ~WageWidget();
    void clearTable();
public slots://公有槽函数
    //按键槽函数
    void ButtonFindPress();
    void ButtonFindPressFinished(QList<WageData*> *dataList);
    void ButtonAddPress();
    void ButtonAddPressEnter(QList<StaffDataList *> *list);
    void ButtonDelPress();
    void ButtonDelAllPress();
    void ButtonDetailsPress();
public://外部数据
    Data *m_data;
    MainWindow *m_parent;
protected://事件
    void keyPressEvent(QKeyEvent *event);
private://私有函数
    void createWidget();
private://私有数据
    QList<WageData *> *m_dataList;
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
    UButton *m_btn4;
    //---查询范围---
    QHBoxLayout *m_findLayout;
    //搜索
    QLabel *m_findLabel;
    QLineEdit *m_edit;
    //时间
    QHBoxLayout *m_dateSLayout;
    QLabel *m_dateSLabel;
    UDateBox *m_dateSBox;
    UButton *m_dateSBtn1;
    UButton *m_dateSBtn2;
    QHBoxLayout *m_dateELayout;
    QLabel *m_dateELabel;
    UDateBox *m_dateEBox;
    UButton *m_dateEBtn1;
    UButton *m_dateEBtn2;
    //售价
    URectBox *m_priceBox;
    //结果
    QLabel *m_result;
    //---子窗口---
    WageItemWidget *m_addWidget;
    WageItemWidget *m_detailsWidget;
};

#endif // WAGEWIDGET_H
