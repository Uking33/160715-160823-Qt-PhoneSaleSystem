#ifndef SHOPINGWIDGET_H
#define SHOPINGWIDGET_H

//窗口
#include <QWidget>
class MainWindow;
//布局
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
//控件
class UButton;
class QTableWidget;
class QLabel;
class QComboBox;
class QPushButton;
//数据
class Data;
class PhoneData;
class ToolData;
class PhoneRecord;
class PhoneRecordList;
//辅助
class QTableWidgetSelectionRange;

class ShopingWidget:public QWidget
{
    Q_OBJECT
public://公有函数
    ShopingWidget(MainWindow *parent, Data *data, bool edit);
    ~ShopingWidget();
    void clearTable();//重置
    int getCount();//获取项目数
    int getToolCount(QString name);//获取配件数量
    bool containsData(const QString str);//查询是否存在于购物车
    bool insertData(PhoneData*data);//添加手机
    bool insertData(ToolData* data);//添加配件
    void reHitWidget();//刷新弹出框
public slots://公有槽函数
    void ButtonPrintPress();//打印
    void ButtonEditPressFinished(PhoneRecord *data);//修改结束
protected://事件
    void keyPressEvent(QKeyEvent *event);
private://私有函数
    void createWidget();//创建
    void updateTable();//更新表格
    void clearData();//清除数据
private slots: //私有槽函数
    void ButtonEnterPrintPress();//确定打印
    void ButtonEnterPrintPressFinished();//开始打印
    void ButtonAddToolPress();//添加配件
    void ButtonClearPress();//清除购物车
    void ButtonEditPress();//修改
    void ButtonDelPress();//删除
private://私有数据
    int m_col;
private://外部数据
    bool m_model;
    Data *m_data;
    MainWindow *m_parent;
private://私有控件
    QList<int> m_IDList;
    QList<QTableWidgetSelectionRange> m_selectionRange;
    PhoneRecordList *m_record;
    //窗口
    QVBoxLayout *m_layout;
    QHBoxLayout *m_btnLayout;
    UButton *m_btn1;
    UButton *m_btn2;
    UButton *m_btn3;
    UButton *m_btn4;
    UButton *m_btn5;
    QTableWidget *m_table;
    //----打印----
    QDialog *m_hitwidget;
    QGridLayout *m_hitlayout;
    QLabel *m_hitlabel;
    QLabel *m_hitEditlabel1;
    QComboBox *m_hitEdit1;
    QLabel *m_hitEditlabel2;
    QComboBox *m_hitEdit2;
    QPushButton *m_hitbtn1;
    QPushButton *m_hitbtn2;
};

#endif // SHOPINGWIDGET_H
