#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QDebug>
#include <QMainWindow>
class QTabWidget;
class PhoneWidget;
class ToolWidget;
class ShopingWidget;
class RecordWidget;
class StaffWidget;
class ErrorWidget;
class WageWidget;
class AssetsWidget;
class PrintSum;
class PrintWidget;
class SettingWidget;
class Data;
class IniData;
class PhoneData;
class ToolData;
class PhoneRecord;
class ULoading;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(Data *data, IniData *ini, QString ID, QString name, QWidget *parent = 0);
    ~MainWindow();
    void keyPress(QKeyEvent *);
    void mouseMove(QMouseEvent *);
    void mousePress(QMouseEvent *);
    void mouseRelease(QMouseEvent *);
    ULoading *m_loading;
    QString m_ID;//当前账号
    QString m_name;//当前账号
    IniData *m_ini;
    bool m_open;
    //打印
    PrintSum *m_printsum;
    //接口
    void setSize(int width, int height);
    bool addShopingItem(PhoneData* data);//添加手机
    bool addShopingItem(ToolData* data);//添加配件
    void addShopingItemAndSum(PhoneData* data);//添加手机并出售
    void addShopingItemAndSum();//添加配件并出售
    bool shopingFind(const QString str);//查询是否存在
    bool shopingCount(int count);//查询项目数量
    int shopingToolCount(QString name);//查询配件数量
    void shopingEnd();//出售后处理
    void shopingDel(QStringList IDList, QStringList nameList);//
    void recordUnSaleEnd();
    void updateStaff();    
    void errorEnd();
    void Find(QString type);

public slots:
    //外部查询函数函数
    void ItemDetails(QString type, const QString ID);
    void ItemDetails(QString type,PhoneRecord *data);
    void ItemDetailsEditFinished(QString type, PhoneRecord *data);
    void ItemDetailsEdit(QString type, PhoneRecord *data);
    void ItemShop(QString type, const QString &str);
    //切换函数
    void choosePhone();
    void chooseTool();
    void chooseShoping();
    void chooseRecord();
    void chooseStaff();
    void chooseError();
    void chooseWage();
    void chooseAssets();
    void Setting();
    void EditPassword();
    void Logout();
    void Close();
    void About();
    //弹框
    void MessageHit(const QString &text);
    void MessageHit(const QString &text, int msec);
    bool MessageQuestion(const QString &text);
private:
    //定时器
    QTimer *timer;
    //创建
    void createWidget();     //创建窗口
    void createMenus();     //创建菜单
    void createActions();   //创建动作
    //响应动作
    QAction *m_phoneAction;
    QAction *m_toolAction;
    QAction *m_shopingAction;
    QAction *m_recordAction;
    QAction *m_staffAction;
    QAction *m_assetsAction;
    QAction *m_wageAction;
    QAction *m_settingAction;
    QAction *m_errorAction;
    QAction *m_editPasswordAction;
    QAction *m_logoutAction;
    QAction *m_closeAction;
    QAction *m_buyingPriceAction;
    QAction *m_salingPriceAction;
    QAction *m_gatherAction;
    QAction *m_aboutAction;
    //菜单
    QMenu *file1;
    QMenu *file2;
    QMenu *file3;
    //窗口
    PhoneWidget *m_phoneWidget;
    ToolWidget *m_toolWidget;
    ShopingWidget *m_shopingWidget;
    RecordWidget *m_recordWidget;
    StaffWidget *m_staffWidget;
    ErrorWidget *m_errorWidget;
    WageWidget *m_wageWidget;
    AssetsWidget *m_assetsWidget;
    QTabWidget *m_centerWindow;
    //设置
    SettingWidget *m_settingWidget;
    //数据
    Data *m_data;
    bool m_model;//当前模式
};

#endif // MAINWINDOW_H
