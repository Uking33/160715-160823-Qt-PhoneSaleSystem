#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

//窗口
#include <QWidget>
class MainWindow;
class ToolItemWidget;
//布局
class QVBoxLayout;
class QHBoxLayout;
//控件
class QTableWidget;
class QPushButton;
class QLabel;
class QLineEdit;
class UButton;
class ULine;
class UDateBox;
class UTimeBox;
class URectBox;
//数据
class Data;
class ToolData;
class PhoneRecord;
//辅助
class QTableWidgetSelectionRange;

class ToolWidget : public QWidget{
    Q_OBJECT
public:
    ToolWidget(MainWindow *parent, Data *data, bool model);
    ~ToolWidget();
    void clearTable();
    void ItemDetailsFind(PhoneRecord *data);//外部强制查询
    void ItemDetailsEdit(PhoneRecord *data);//修改外部配件
    void ItemShop(const QString &name);//外部出售
    void UnSale(QStringList list);//取消添加出售
public slots://按键槽函数
    void ItemShopFinished(QList<ToolData *>* dataList);//外部出售

    void ButtonFindPressFinished(QList<ToolData *> *dataList);
    void ButtonFindPress();
    void ButtonAddPress();
    void ButtonAddPressEnter();
    void ButtonDelPress();
    void ButtonEditPress();
    void ButtonEditPressEnter();
    void ButtonDetailsPress();
    void ButtonDetailsSaleAddPress();
    void ButtonDetailsSaleSumPress();
    void ButtonPrintPress();
public://外部数据
    bool m_out;
    int m_printCount;
    bool m_model;
    Data *m_data;
    MainWindow *m_parent;
protected://事件
    void keyPressEvent(QKeyEvent *event);
private://私有函数
    void createWidget();
private://私有数据
    //数据
    int m_col;
    QList<ToolData *> *m_dataList;
    //辅助数据
    bool m_printIsFree;
    QStringList m_IDList;
    QList<QTableWidgetSelectionRange> m_selectionRange;

public://公有控件
    //---布局---
    QVBoxLayout *m_layout;
    //--表格--
    QTableWidget *m_table;
    //--查询范围--
    QHBoxLayout *m_findLayout;
    URectBox *m_buyingBox;//成本价
    URectBox* m_saleBox;//零售价
    QLabel *m_findLabel;//搜索
    QLineEdit *m_findEdit;//搜索
    QLabel *m_result;//结果
    //--按钮--
    QHBoxLayout *m_btnLayout;
    UButton *m_btn1;
    UButton *m_btn2;
    UButton *m_btn3;
    UButton *m_btn4;
    UButton *m_btn5;
    //---数字框---
    QDialog *p_dlg;
    QVBoxLayout *p_layout;
    QHBoxLayout *p_btnLayout;
    ULine *p_count;
    QPushButton *p_btn1;
    QPushButton *p_btn2;
    QPushButton *p_btn3;
    //---子窗口---
    ToolItemWidget *m_addWidget;
    ToolItemWidget *m_editWidget;
    ToolItemWidget *m_detailsWidget;
};

#endif // TOOLWIDGET_H
