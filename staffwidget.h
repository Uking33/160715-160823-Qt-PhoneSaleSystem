#ifndef STAFFWIDGET_H
#define STAFFWIDGET_H

//窗口
#include <QWidget>
class StaffItemWidget;
class MainWindow;
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
class QComboBox;
//数据
class Data;
class StaffDataList;
//辅助
class QTableWidgetSelectionRange;

class StaffWidget : public QWidget{
    Q_OBJECT
protected://事件
    void keyPressEvent(QKeyEvent *event);
public://公有函数
    StaffWidget(MainWindow *parent, Data *data);
    ~StaffWidget();
    void clearTable();
    void ItemDetailsFind(const QString ID);//外部强制查询
public slots://公有槽函数
    void ItemDetailsFindFinished(QList<StaffDataList *>* dataList);//外部强制查询-结束槽函数
    //按键槽函数
    void ButtonFindPress();
    void ButtonFindPressFinished(QList<StaffDataList *> *dataList);
    void ButtonAddPress();
    void ButtonAddPressEnter();
    void ButtonDelPress();
    void ButtonEditPress();
    void ButtonEditPressEnter();
    void ButtonDetailsPress();
public://公有数据
    bool m_newFind;
public://外部数据
    Data *m_data;
    MainWindow *m_parent;
public://公有控件
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
    QHBoxLayout *m_editLayout;
    QLabel *m_findLabel;
    QLineEdit *m_edit;
    //业绩时间
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
    //员工姓名
    QHBoxLayout *m_nameLayout;
    QLabel *m_nameLabel;
    QComboBox *m_nameBox;
    //结果
    QLabel *m_result;
    //---子窗口---
    StaffItemWidget *m_addWidget;
    StaffItemWidget *m_editWidget;
    StaffItemWidget *m_detailsWidget;
private://私有函数
    void createWidget();
private://私有数据
    QList<StaffDataList *> *m_dataList;
    QStringList m_IDList;
    QList<QTableWidgetSelectionRange> m_selectionRange;
};


#endif // STAFFWIDGET_H
