#ifndef FINDWIDGET_H
#define FINDWIDGET_H

//窗口
#include <QWidget>
class MainWindow;
class PhoneItemWidget;
class PhoneLoading;
//布局
class QVBoxLayout;
class QHBoxLayout;
//控件
class QTableWidget;
class QLabel;
class QComboBox;
class QLineEdit;
class QCheckBox;
class UButton;
class UDateBox;
class UTimeBox;
class ULine;
class URectBox;
//数据
class Data;
class PhoneData;
class PhoneRecord;
//辅助
class QTableWidgetSelectionRange;

class PhoneWidget: public QWidget
{
    Q_OBJECT
public://公有函数
    PhoneWidget(MainWindow *parent, Data *data, bool edit);
    ~PhoneWidget();
    void clearTable();
    void ItemDetailsEdit(PhoneRecord *data);//外部编辑
    void ItemDetailsFind(const QString ID);//外部查询
public://公有数据
    PhoneLoading *m_loading;
public://外部数据
    Data *m_data;
    MainWindow *m_parent;
    QTableWidget *m_findTable;
protected:
    void keyPressEvent(QKeyEvent *event);
private://私有函数
    //创建
    void createWidget();
    void createAddWidget();
    void createEditWidget();
    void createDetailsWidget();
    //选择
    void chooseAddWidget();
    void chooseEditWidget();
    void chooseDelWidget();
    void chooseDetailsWidget();
    //重置获取
    void reAddWidget();
    void reEditWidget();
private slots://私有槽函数
    void ItemDetailsEditFinished(QList<PhoneData *>* dataList);//外部编辑查找结束
    void ItemDetailsFindFinished(QList<PhoneData *>* dataList);//外部强制查询-结束槽函数
    //按键槽函数
    void ButtonFindPressFinished(QList<PhoneData *> *dataList);//查询结束
    void ButtonFindPress();
    void ButtonShopPress();
    void ButtonAddPress();
    void ButtonEditPress();
    void ButtonDelPress();
    void ButtonAddOnePress();
    void ButtonAddBackPress();
    void ButtonEditEnterPress();
    void ButtonEditBackPress();
    void ButtonDetailsPress();
    void ButtonDetailsSaleAddPress();
    void ButtonDetailsSaleSumPress();
    void ButtonDetailsBackPress();
    void ButtonPrintPress();
private://私有数据
    bool m_out;
    bool m_model;
    int m_col;
    QList<PhoneData*> *m_dataList;
private://私有控件
    //---主界面-查询界面---
    QVBoxLayout *m_layout;
    //通用控件
    QHBoxLayout *m_findConLayout;
    QLabel *m_findIndateSLabel;
    QHBoxLayout *m_findIndateSLayout;
    UDateBox *m_findIndateSBox;
    UButton *m_findIndateSBtn1;
    UButton *m_findIndateSBtn2;
    URectBox *m_findSaleBox;
    QLabel *m_findLabel;
    QLineEdit *m_findEdit;
    //管理员控件
    QHBoxLayout *m_saleLayout;
    QLabel *m_saleLabel;
    QCheckBox *m_saleCheck;
    QLabel *m_result;
    QHBoxLayout *m_findAdLayout;
    QHBoxLayout *m_findInDateELayout;
    QLabel *m_findInDateELabel;
    UDateBox *m_findInDateEBox;
    UButton *m_findInDateEBtn1;
    UButton *m_findInDateEBtn2;
    URectBox *m_findBuyBox;
    //按钮
    QHBoxLayout *m_findBtnLayout;
    UButton *m_findBtn1;
    UButton *m_findBtn2;
    UButton *m_findBtn3;
    UButton *m_findBtn4;
    UButton *m_findBtn5;
    UButton *m_findBtn6;
    //---添加界面---
    PhoneItemWidget *m_addWidget;
    //---编辑界面---
    PhoneItemWidget *m_editWidget;
    PhoneData * t_data;
    QStringList m_IDList;
    QList<QTableWidgetSelectionRange> m_selectionRange;
    //---详情界面---
    PhoneItemWidget *m_detailsWidget;
};

#endif // FINDWIDGET_H
