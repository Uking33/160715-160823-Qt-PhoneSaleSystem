#ifndef ASSETSWIDGET_H
#define ASSETSWIDGET_H

//窗口
#include <QWidget>
class MainWindow;
class AssetsItemWidget;
//布局
class QVBoxLayout;
class QHBoxLayout;
//控件
class QTableWidget;
class QLabel;
class UButton;
class ULine;
class URectBox;
class QLineEdit;
//数据
class Data;
class AssetsData;
//辅助
class QTableWidgetSelectionRange;

class AssetsWidget : public QWidget{
    Q_OBJECT
public://公有函数
    AssetsWidget(MainWindow *parent, Data *data);
    ~AssetsWidget();
    void clearTable();
public slots://公有按键槽函数
    void ButtonFindPressFinished(QList<AssetsData *> *dataList);
    //按键槽函数
    void ButtonFindPress();
    void ButtonAddPress();
    void ButtonAddPressEnter();
    void ButtonEditPress();
    void ButtonEditPressEnter();
    void ButtonDelPress();
    void ButtonDelAllPress();
    void ButtonDetailsPress();
public://外部数据
    MainWindow *m_parent;
    Data *m_data;
public://公有数据
    QList<AssetsData *> *m_dataList;
protected://事件
    void keyPressEvent(QKeyEvent *event);
private://私有函数
    void createWidget();
private://私有数据
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
    UButton *m_btn5;
    //---查询范围---
    QHBoxLayout *m_findLayout;
    //搜索
    QLabel *m_findLabel;
    QLineEdit *m_edit;
    //开单时间
    URectBox *m_oneBox;
    URectBox *m_allBox;
    QLabel *m_result;
    //---子窗口---
    AssetsItemWidget *m_addWidget;
    AssetsItemWidget *m_editWidget;
    AssetsItemWidget *m_detailsWidget;
};

#endif // ASSETSWIDGET_H
