//窗口
#include "mainwindow.h"
#include "loginwidget.h"
#include "phonewidget.h"
#include "toolwidget.h"
#include "shopingwidget.h"
#include "recordwidget.h"
#include "staffwidget.h"
#include "errorwidget.h"
#include "wagewidget.h"
#include "assetswidget.h"
#include "uloading.h"
#include "phoneloading.h"
#include "printsum.h"
#include "printwidget.h"
#include "settingwidget.h"
#include <QTabWidget>
#include <QAction>
#include <QMenuBar>
//数据
#include "data.h"
#include "inidata.h"
//辅助
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>
#include <QInputDialog>
#include <QLineEdit>
#include <QCheckBox>

MainWindow::MainWindow(Data *data, IniData *ini, QString ID, QString name, QWidget *parent) :
    QMainWindow(parent)
{
    //初始化
    m_data=data;
    m_ID=ID;
    m_name=name;    
    //姓名
    if(m_model && m_ID=="0000" &&m_name=="管理员"){
        bool ok;
        QInputDialog input;
        QString text;
        while(1){
            text=input.getText(this,"姓名输入","管理员，您好。您第一次登陆，请输入您的姓名",QLineEdit::Normal,"",&ok);
            if (ok){
                if(text.isEmpty())
                    MessageHit("姓名不能为空");
                else if(text=="管理员")
                    MessageHit("姓名不能为管理员");
                else{
                    QStringList strList; strList.append("name");
                    StaffDataList* data=new StaffDataList; data->name=text; data->ID=m_ID;
                    m_data->Update(m_ID,m_name,strList,data);
                    delete data;
                    m_name=text;
                    m_open=true;
                    break;
                }
            }
            else{
                m_open=false;
                return;
            }
        }
    }
    setWindowTitle("手机管理系统 工号:"+m_ID+" 姓名:"+m_name);    
    m_open=true;
    if(ID=="0000")
        m_model=true;
    else m_model=false;
    connect(m_data, SIGNAL(SendMessageHit(QString)), this, SLOT(MessageHit(QString)));
    m_ini=ini;
    //窗口
    m_centerWindow=NULL;
    m_phoneWidget=NULL;
    m_shopingWidget=NULL;
    m_recordWidget=NULL;
    m_staffWidget=NULL;
    m_assetsWidget=NULL;
    m_errorWidget=NULL;
    createWidget();
    createActions();
    createMenus();
    m_loading = new ULoading(m_data);
    choosePhone();
    //检验密码
    if(m_model && m_data->Login(m_ID,m_ID)==1){
        MessageHit("工号和密码相同,请尽快修改密码!");
    }
    //定时器
    if(m_model){
        setMouseTracking(true);
        m_centerWindow->setMouseTracking(true);
        //定时器
        timer = new QTimer(this);
        connect(timer,&QTimer::timeout, [=](){
            timer->stop();
            LogInWidget log(m_data,m_ini,4,m_ID);
            if(log.exec()==0)
                this->Close();
            else
                this->raise();
        });
        timer->start(60000*5);
    }
}

MainWindow::~MainWindow(){
    if(m_open){
        delete m_printsum;
        delete m_centerWindow;
    }
}
//创建
void MainWindow::createWidget(){//创建窗口
    //打印
    m_printsum=new PrintSum(this);
    //设置
    m_settingWidget=new SettingWidget(this,m_ini);
    //窗口
    m_centerWindow=new QTabWidget();
    m_phoneWidget=new PhoneWidget(this,m_data,m_model);
    m_centerWindow->addTab(m_phoneWidget,"手机");

    m_toolWidget=new ToolWidget(this,m_data,m_model);
    m_centerWindow->addTab(m_toolWidget,"配件");

    m_shopingWidget=new ShopingWidget(this,m_data,m_model);
    m_centerWindow->addTab(m_shopingWidget,"购物车");

    if(m_model){
        m_recordWidget=new RecordWidget(this,m_data);
        m_centerWindow->addTab(m_recordWidget,"销售记录");

        m_errorWidget=new ErrorWidget(this,m_data);
        m_centerWindow->addTab(m_errorWidget,"分摊费用");

        m_staffWidget=new StaffWidget(this,m_data);
        m_centerWindow->addTab(m_staffWidget,"员工管理");

        m_wageWidget=new WageWidget(this,m_data);
        m_centerWindow->addTab(m_wageWidget,"工资管理");

        m_assetsWidget=new AssetsWidget(this,m_data);
        m_centerWindow->addTab(m_assetsWidget,"固定资产");
    }    

    if(m_model){
        m_centerWindow->setTabToolTip(0,"查询/管理手机库存");
        m_centerWindow->setTabToolTip(1,"查询/管理配件库存");
        m_centerWindow->setTabToolTip(2,"查看购物车");
        m_centerWindow->setTabToolTip(3,"查看/管理销售记录");
        m_centerWindow->setTabToolTip(4,"查看/管理分摊费用");
        m_centerWindow->setTabToolTip(5,"查看/管理员工");
        m_centerWindow->setTabToolTip(6,"查看/管理工资");
        m_centerWindow->setTabToolTip(7,"查看/管理固定资产");
        connect(m_centerWindow,&QTabWidget::currentChanged,[=](int index){
            switch(index){
                case 0:choosePhone();break;
                case 1:chooseTool();break;
                case 2:chooseShoping();break;
                case 3:chooseRecord();break;
                case 4:chooseError();break;
                case 5:chooseStaff();break;
                case 6:chooseWage();break;
                case 7:chooseAssets();break;
            }
        });
    }
    else{
        m_centerWindow->setTabToolTip(0,"查询手机库存");
        m_centerWindow->setTabToolTip(1,"查询/管理配件库存");
        m_centerWindow->setTabToolTip(2,"查看购物车");
        connect(m_centerWindow,&QTabWidget::currentChanged,[=](int index){
            switch(index){
                case 0:choosePhone();break;
                case 1:chooseTool();break;
                case 2:chooseShoping();break;
            }
        });
    }

    m_centerWindow->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(m_centerWindow);    
}
void MainWindow::createActions(){//创建动作
    m_phoneAction = new QAction("手机", this);
    m_phoneAction->setShortcut(QKeySequence(tr("Alt+1")));
    connect(m_phoneAction, &QAction::triggered, this, &MainWindow::choosePhone);

    m_toolAction = new QAction("配件", this);
    m_toolAction->setShortcut(QKeySequence(tr("Alt+2")));
    connect(m_toolAction, &QAction::triggered, this, &MainWindow::chooseTool);

    m_shopingAction = new QAction("购物车", this);
    m_shopingAction->setShortcut(QKeySequence(tr("Alt+3")));
    connect(m_shopingAction, &QAction::triggered, this, &MainWindow::chooseShoping);

    if(m_model){
        m_recordAction = new QAction("销售记录", this);
        m_recordAction->setShortcut(QKeySequence(tr("Alt+4")));
        connect(m_recordAction, &QAction::triggered, this, &MainWindow::chooseRecord);

        m_errorAction = new QAction("分摊费用", this);
        m_errorAction->setShortcut(QKeySequence(tr("Alt+5")));
        connect(m_errorAction, &QAction::triggered, this, &MainWindow::chooseError);

        m_staffAction = new QAction("员工管理", this);
        m_staffAction->setShortcut(QKeySequence(tr("Alt+6")));
        connect(m_staffAction, &QAction::triggered, this, &MainWindow::chooseStaff);

        m_wageAction = new QAction("工资管理", this);
        m_wageAction->setShortcut(QKeySequence(tr("Alt+7")));
        connect(m_wageAction, &QAction::triggered, this, &MainWindow::chooseError);

        m_assetsAction = new QAction("固定资产", this);
        m_assetsAction->setShortcut(QKeySequence(tr("Alt+8")));
        connect(m_assetsAction, &QAction::triggered, this, &MainWindow::chooseAssets);

        m_settingAction = new QAction("出单设置", this);
        connect(m_settingAction, &QAction::triggered, this, &MainWindow::Setting);
    }

    m_editPasswordAction = new QAction("修改密码", this);
    connect(m_editPasswordAction, &QAction::triggered, this, &MainWindow::EditPassword);

    m_logoutAction = new QAction("注销", this);
    connect(m_logoutAction, &QAction::triggered, this, &MainWindow::Logout);

    m_closeAction = new QAction("关闭", this);
    m_closeAction->setShortcut(QKeySequence(tr("Alt+F4")));
    connect(m_closeAction, &QAction::triggered, this, &MainWindow::Close);

    if(m_model){
        m_buyingPriceAction = new QAction("总进货价", this);
        connect(m_buyingPriceAction, &QAction::triggered,[=](){
            Find("buyingPrice");
        });
        m_salingPriceAction = new QAction("总销售价", this);
        connect(m_salingPriceAction, &QAction::triggered,[=](){
            Find("salingPrice");
        });
        m_gatherAction = new QAction("总毛利", this);
        connect(m_gatherAction, &QAction::triggered,[=](){
            Find("gather");
        });
    }

    m_aboutAction = new QAction("关于", this);
    m_aboutAction->setShortcut(QKeySequence(tr("Alt+F1")));
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::About);
}

void MainWindow::createMenus(){//创建菜单    
    menuBar()->clear();
    file1 = menuBar()->addMenu("菜单");
    file1->addAction(m_phoneAction);
    file1->addAction(m_toolAction);
    file1->addAction(m_shopingAction);
    if(m_model){
        file1->addAction(m_recordAction);
        file1->addAction(m_errorAction);
        file1->addAction(m_staffAction);
        file1->addAction(m_wageAction);
        file1->addAction(m_assetsAction);
    }
    file1->addSeparator();
    if(m_model) file1->addAction(m_settingAction);
    file1->addAction(m_editPasswordAction);
    file1->addAction(m_logoutAction);    
    file1->addSeparator();
    file1->addAction(m_closeAction);

    if(m_model){
        file2 = menuBar()->addMenu("查询");
        file2->addAction(m_buyingPriceAction);
        file2->addAction(m_salingPriceAction);
        file2->addAction(m_gatherAction);
    }

    file3 = menuBar()->addMenu("其他");
    file3->addAction(m_aboutAction);
}

//接口
void MainWindow::setSize(int width,int height){
    QDesktopWidget *pDesk = QApplication::desktop();
    int h = pDesk->availableGeometry().height();
    int w = pDesk->availableGeometry().width();
    if(width==0)
        width=w;
    if(height==0)
        height=h;
    setFixedSize(width,height);
    move((w-width)/2, (h-height)/2);
}

bool MainWindow::addShopingItem(PhoneData* data){
    if(m_shopingWidget->insertData(data)){
        return true;
    }
    else{
        return false;
    }
}

bool MainWindow::addShopingItem(ToolData* data){
    if(m_shopingWidget->insertData(data)){
        return true;
    }
    else{
        return false;
    }
}

void MainWindow::addShopingItemAndSum(PhoneData* data){
    if(!m_shopingWidget->insertData(data)){
        MessageHit("添加失败!");
        return;
    }
    QMessageBox message(QMessageBox::Information, "提示", QString("已添加这个物品到购物车"));
    message.show();
    QTimer::singleShot(1000,&message,&QMessageBox::close);
    message.exec();
    chooseShoping();
    m_shopingWidget->ButtonPrintPress();
}

void MainWindow::addShopingItemAndSum(){
    m_toolWidget->ButtonPrintPress();
    if(m_toolWidget->m_printCount>0){
        chooseShoping();
        m_shopingWidget->ButtonPrintPress();
    }
}

void MainWindow::shopingEnd(){
    m_phoneWidget->clearTable();
    m_toolWidget->clearTable();
    if(m_model){
        m_recordWidget->clearTable();
        m_staffWidget->m_newFind=true;
        m_staffWidget->clearTable();
    }
}

bool MainWindow::shopingFind(const QString str){
    if(m_shopingWidget->containsData(str))
        return true;
    else
        return false;
}

bool MainWindow::shopingCount(int count){
    if(m_shopingWidget->getCount()+count>4)
        return false;
    return true;
}

void MainWindow::shopingDel(QStringList IDList,QStringList nameList){
    //清手机
    if(m_model)
        m_phoneWidget->m_loading->Start("SALEOUT",IDList.count(),NULL,NULL,IDList);
    //清配件
    m_toolWidget->UnSale(nameList);
}

int MainWindow::shopingToolCount(QString name){
    return m_shopingWidget->getToolCount(name);
}

void MainWindow::recordUnSaleEnd(){
    m_phoneWidget->clearTable();
    m_toolWidget->clearTable();
    m_staffWidget->m_newFind=true;
    m_staffWidget->ButtonFindPress();
}

void MainWindow::errorEnd(){
    m_staffWidget->m_newFind=true;
    if(m_staffWidget->m_table->item(0,0)!=NULL)
        m_staffWidget->ButtonFindPress();
}

void MainWindow::ItemDetails(QString type,const QString ID){
    if(type=="phone")
        m_phoneWidget->ItemDetailsFind(ID);
    else if(type=="record")
        m_recordWidget->ItemDetailsFind(ID);
    else if(type=="error")
        m_errorWidget->ItemDetailsFind(ID);
    else if(type=="staff")
        m_staffWidget->ItemDetailsFind(ID);
}

void MainWindow::ItemDetails(QString type,PhoneRecord *data){
    if(type=="tool")
        m_toolWidget->ItemDetailsFind(data);
}

void MainWindow::ItemDetailsEdit(QString type, PhoneRecord *data){
    if(type=="tool")
        m_toolWidget->ItemDetailsEdit(data);
    else if(type=="phone")
        m_phoneWidget->ItemDetailsEdit(data);
}
void MainWindow::ItemDetailsEditFinished(QString type, PhoneRecord *data){
    if(type=="tool")
        m_shopingWidget->ButtonEditPressFinished(data);
    else if(type=="phone")
        m_shopingWidget->ButtonEditPressFinished(data);
}

void MainWindow::ItemShop(QString type,const QString &str){
    if(type=="tool")
        m_toolWidget->ItemShop(str);
}

void MainWindow::updateStaff(){
    m_shopingWidget->reHitWidget();
}

//消息槽函数
void MainWindow::MessageHit(const QString &text){
    QMessageBox message(QMessageBox::Information, "提示", text, QMessageBox::Ok, this);
    message.exec();
}
void MainWindow::MessageHit(const QString &text,int msec){
    QMessageBox message(QMessageBox::Information, "提示", text, QMessageBox::Ok, this);
    if(msec!=0){
        message.show();
        QTimer::singleShot(msec,&message,&QMessageBox::close);
    }
    message.exec();
}

bool MainWindow::MessageQuestion(const QString &text){
    QMessageBox box(QMessageBox::Question,"提示",text);
    box.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
    box.setButtonText(QMessageBox::Ok,"是");
    box.setButtonText(QMessageBox::Cancel,"否");
    if(box.exec()==QMessageBox::Ok)
        return true;
    return false;

}

void MainWindow::Setting(){
    m_settingWidget->reWidget();
    m_settingWidget->exec();
}

void MainWindow::EditPassword(){//修改密码
    LogInWidget log1(m_data,m_ini,1,m_ID);
    if(log1.exec()==1){
        LogInWidget log2(m_data,m_ini,2,m_ID,log1.GetPassword());
        if(log2.exec()==1){
            LogInWidget log3(m_data,m_ini,3,m_ID,log2.GetPassword());
            if(log3.exec()==1){
                //修改密码
                m_data->EditPassword(m_ID,log3.GetPassword());
                MessageHit("密码修改成功，请记住新密码!");
            }
        }
    }
}

void MainWindow::Logout(){//注销
    QApplication::exit(1);
}

void MainWindow::Close(){//关闭
    QApplication::exit(0);
}

void MainWindow::About(){//关于
    QString str="手机管理系统1.0\n开发人员:UKing\nQQ:574867885\n若此软件在运行过程中有非功能性问题，请联系开发者";
    QMessageBox message(QMessageBox::Information, "关于", str, QMessageBox::Ok, this);
    message.exec();
}

void MainWindow::Find(QString type){
    double sum=0;
    int i;
    QString text;
    QTableWidget *table;
    if(type=="buyingPrice"){
        if(m_centerWindow->currentIndex()==0){
            text="总进货价";
            table=m_phoneWidget->m_findTable;
            for(i=0;i<table->rowCount();i++)
                if(table->item(i,0)!=NULL)
                    sum+=table->item(i,3)->text().remove(0,1).toDouble();
        }
        else if(m_centerWindow->currentIndex()==1){
            text="总成本价";
            table=m_toolWidget->m_table;
            for(i=0;i<table->rowCount();i++)
                if(table->item(i,0)!=NULL)
                    sum+=table->item(i,2)->text().remove(0,1).toDouble()*table->item(i,1)->text().toInt();
        }
    }
    else if(type=="salingPrice"){
        if(m_centerWindow->currentIndex()==0){
            table=m_phoneWidget->m_findTable;
            text="总销售价";
            for(i=0;i<table->rowCount();i++)
                if(table->item(i,0)!=NULL)
                    sum+=table->item(i,4)->text().remove(0,1).toDouble();
        }
        else if(m_centerWindow->currentIndex()==1){
            table=m_toolWidget->m_table;
            text="总零售价";
            for(i=0;i<table->rowCount();i++)
                if(table->item(i,0)!=NULL)
                    sum+=table->item(i,3)->text().remove(0,1).toDouble()*table->item(i,1)->text().toInt();
        }
    }
    else if(type=="gather"){
        text="总毛利";
        if(m_centerWindow->currentIndex()==0){
            table=m_phoneWidget->m_findTable;
            for(i=0;i<table->rowCount();i++)
                if(table->item(i,0)!=NULL)
                    sum+=table->item(i,4)->text().remove(0,1).toDouble()-table->item(i,3)->text().remove(0,1).toDouble();
        }
        else if(m_centerWindow->currentIndex()==1){
            table=m_toolWidget->m_table;
            for(i=0;i<table->rowCount();i++)
                if(table->item(i,0)!=NULL)
                    sum+=(table->item(i,3)->text().remove(0,1).toDouble()-table->item(i,2)->text().remove(0,1).toDouble())*table->item(i,1)->text().toInt();
        }
    }
    QMessageBox message(QMessageBox::Information, "总和", text+"为￥"+QString::number(sum,'d',2), QMessageBox::Ok, this);
    message.exec();
}

//切换接口
void MainWindow::choosePhone(){
    if(m_model){
        m_buyingPriceAction->setEnabled(true);
        m_salingPriceAction->setEnabled(true);
        m_gatherAction->setEnabled(true);
    }
    m_centerWindow->setCurrentIndex(0);
}

void MainWindow::chooseTool(){
    if(m_model){
        m_buyingPriceAction->setEnabled(true);
        m_salingPriceAction->setEnabled(true);
        m_gatherAction->setEnabled(true);
        m_centerWindow->setCurrentIndex(1);
    }
}

void MainWindow::chooseShoping(){
    if(m_model){
        m_buyingPriceAction->setEnabled(false);
        m_salingPriceAction->setEnabled(false);
        m_gatherAction->setEnabled(false);
        m_centerWindow->setCurrentIndex(2);
    }
}

void MainWindow::chooseRecord(){
    m_buyingPriceAction->setEnabled(false);
    m_salingPriceAction->setEnabled(false);
    m_gatherAction->setEnabled(false);
    m_centerWindow->setCurrentIndex(3);
}

void MainWindow::chooseError(){
    m_buyingPriceAction->setEnabled(false);
    m_salingPriceAction->setEnabled(false);
    m_gatherAction->setEnabled(false);
    m_centerWindow->setCurrentIndex(4);
}

void MainWindow::chooseStaff(){
    m_buyingPriceAction->setEnabled(false);
    m_salingPriceAction->setEnabled(false);
    m_gatherAction->setEnabled(false);
    m_centerWindow->setCurrentIndex(5);
}

void MainWindow::chooseWage(){
    m_buyingPriceAction->setEnabled(false);
    m_salingPriceAction->setEnabled(false);
    m_gatherAction->setEnabled(false);
    m_centerWindow->setCurrentIndex(6);
}

void MainWindow::chooseAssets(){
    m_buyingPriceAction->setEnabled(false);
    m_salingPriceAction->setEnabled(false);
    m_gatherAction->setEnabled(false);
    m_centerWindow->setCurrentIndex(7);
}

void MainWindow::mouseMove(QMouseEvent *){
    if(m_model && timer!=NULL){
        timer->stop();
        timer->start(60000*5);
    }
}
void MainWindow::mousePress(QMouseEvent *){
    if(m_model && timer!=NULL){
        timer->stop();
        timer->start(60000*5);
    }
}
void MainWindow::mouseRelease(QMouseEvent *){
    if(m_model && timer!=NULL){
        timer->stop();
        timer->start(60000*5);
    }
}

void MainWindow::keyPress(QKeyEvent *){
    if(m_model && timer!=NULL){
        timer->stop();
        timer->start(60000*5);
    }
}

