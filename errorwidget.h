#ifndef ERRORWIDGET_H
#define ERRORWIDGET_H


//窗口
#include <QWidget>
class ErrorItemWidget;
class MainWindow;
//布局
class QVBoxLayout;
class QHBoxLayout;
//控件
class QTableWidget;
class QLineEdit;
class QLabel;
class UButton;
class ULine;
class UDateBox;
class URectBox;
//数据
class Data;
class ErrorData;
//数据
class QTableWidgetSelectionRange;

class ErrorWidget : public QWidget{
    Q_OBJECT
protected://事件
    void keyPressEvent(QKeyEvent *event);
public://公有函数
    ErrorWidget(MainWindow *parent, Data *data);
    ~ErrorWidget();
    void clearTable();
public slots://公有槽函数
    void ItemDetailsFindFinished(QList<ErrorData *>* dataList);//外部强制查询-结束槽函数
    void ItemDetailsFind(const QString ID);//外部强制查询
    void ButtonFindPressFinished(QList<ErrorData *> *dataList);
    //按键槽函数
    void ButtonFindPress();
    void ButtonAddPress();
    void ButtonAddPressEnter();
    void ButtonEditPress();
    void ButtonEditPressEnter();
    void ButtonDelPress();
    void ButtonDelAllPress();
    void ButtonDetailsPress();
    void DetailsWidgetUnsale();
public://外部数据
    Data *m_data;
    MainWindow *m_parent;
private://私有数据
    QList<ErrorData *> *m_dataList;
    QStringList m_IDList;
    QList<QTableWidgetSelectionRange> m_selectionRange;
    bool m_out;
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
    UButton *m_btn5;
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

    URectBox *m_priceBox;
    QLabel *m_result;
    //---子窗口---
    ErrorItemWidget *m_addWidget;
    ErrorItemWidget *m_editWidget;
    ErrorItemWidget *m_detailsWidget;
private://私有函数
    void createWidget();
};

#endif // ERRORWIDGET_H
