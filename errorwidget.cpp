//窗口
#include "errorwidget.h"
#include "erroritemwidget.h"
#include "mainwindow.h"
#include "uloading.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
//数据
#include "data.h"
//控件
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include "ubutton.h"
#include "uline.h"
#include "urectbox.h"
#include "udatebox.h"
//辅助
#include <QTableWidgetSelectionRange>
#include <QKeyEvent>
#include <QCheckBox>
#include <QComboBox>

ErrorWidget::ErrorWidget(MainWindow *parent, Data *data)
    : QWidget(parent)
{
    //初始化
    m_parent=parent;
    m_data=data;
    m_dataList=NULL;
    m_out=false;
    createWidget();
    //子界面
    m_addWidget = new ErrorItemWidget(this,"ADD");
    m_editWidget = new ErrorItemWidget(this,"EDIT");
    m_detailsWidget = new ErrorItemWidget(this,"DETAILS");
}

ErrorWidget::~ErrorWidget(){
    clearTable();
    delete m_layout;
}

void ErrorWidget::createWidget(){//创建
    m_layout = new QVBoxLayout();
    //---限定---
    m_findLayout = new QHBoxLayout();
    //起始时间
    m_dateSLabel = new QLabel("分摊起始");
    m_dateSBox = new UDateBox(2);
    m_dateSLayout= new QHBoxLayout();
    m_dateSLayout->addWidget(m_dateSLabel);
    m_dateSLayout->addWidget(m_dateSBox);
    m_dateSBtn1 = new UButton("日","F1",25,25,m_dateSLayout);
    m_dateSBtn1->setFocusPolicy(Qt::NoFocus);
    connect(m_dateSBtn1,&UButton::clicked,m_dateSBox,&UDateBox::Today);
    m_dateSBtn2 = new UButton("周","F2",25,25,m_dateSLayout);
    m_dateSBtn2->setFocusPolicy(Qt::NoFocus);
    connect(m_dateSBtn2,&UButton::clicked,m_dateSBox,&UDateBox::ToWeek);
    m_dateSLayout->setSpacing(5);
    //结束时间
    m_dateELabel = new QLabel("分摊结束");
    m_dateEBox = new UDateBox(3,m_dateSBox);
    m_dateSBox->ChooseDay(m_dateSBox->m_comboBoxDay->currentText());
    m_dateELayout= new QHBoxLayout;
    m_dateELayout->addWidget(m_dateELabel);
    m_dateELayout->addWidget(m_dateEBox);
    m_dateEBtn1 = new UButton("月","F5",25,25,m_dateELayout);
    m_dateEBtn1->setFocusPolicy(Qt::NoFocus);
    connect(m_dateEBtn1,&UButton::clicked,m_dateSBox,&UDateBox::ToMonth);
    m_dateEBtn2 = new UButton("年","F6",25,25,m_dateELayout);
    m_dateEBtn2->setFocusPolicy(Qt::NoFocus);
    connect(m_dateEBtn2,&UButton::clicked,m_dateSBox,&UDateBox::ToYear);
    m_dateELayout->setSpacing(5);
    //费用
    m_priceBox = new URectBox("分摊费用","元","元");
    m_priceBox->m_checkBox->setShortcut(QKeySequence("F3"));
    //资产名称
    m_findLabel = new QLabel("搜索:");
    m_edit = new QLineEdit();
    m_edit->setFocusPolicy(Qt::StrongFocus);
    m_edit->setPlaceholderText("项目名称/分摊原因/分摊人员");
    m_edit->setMaxLength(50);
    m_edit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9\u4e00-\u9fa5]*$"), m_edit));
    //布局
    m_findLayout->addLayout(m_dateSLayout);
    m_findLayout->addWidget(m_priceBox);
    m_findLayout->addWidget(m_findLabel);
    m_findLayout->addWidget(m_edit);
    //---结果---
    m_result = new QLabel();
    m_result->setFixedWidth(250);
    m_result->setAlignment(Qt::AlignCenter);
    m_findLayout->addWidget(m_result);
    //---按钮---
    m_btnLayout = new QHBoxLayout();
    m_btnLayout->addLayout(m_dateELayout);
    //创建按钮
    m_btnLayout->addStretch(1);
    m_btn1 = new UButton("查询(Enter)","",80,30,m_btnLayout);
    m_btnLayout->addStretch(1);
    m_btn2 = new UButton("增加(Insert)","Insert",80,30,m_btnLayout);
    m_btnLayout->addStretch(1);
    m_btn3 = new UButton("修改(Space)","Space",80,30,m_btnLayout);
    m_btnLayout->addStretch(1);
    m_btn4 = new UButton("删除(Delete)","Delete",80,30,m_btnLayout);
    m_btnLayout->addStretch(1);
    m_btn5 = new UButton("清空(Esc)","Esc",80,30,m_btnLayout);
    m_btnLayout->addStretch(1);
    m_btn3->setEnabled(false);
    m_btn4->setEnabled(false);
    m_btn5->setEnabled(false);
    //信号连接
    connect(m_btn1,SIGNAL(clicked()),this,SLOT(ButtonFindPress()));
    connect(m_btn2,SIGNAL(clicked()),this,SLOT(ButtonAddPress()));
    connect(m_btn3,SIGNAL(clicked()),this,SLOT(ButtonEditPress()));
    connect(m_btn4,SIGNAL(clicked()),this,SLOT(ButtonDelPress()));
    connect(m_btn5,SIGNAL(clicked()),this,SLOT(ButtonDelAllPress()));
    m_btn1->setFocusPolicy(Qt::NoFocus);
    m_btn2->setFocusPolicy(Qt::NoFocus);
    m_btn3->setFocusPolicy(Qt::NoFocus);
    m_btn4->setFocusPolicy(Qt::NoFocus);
    m_btn5->setFocusPolicy(Qt::NoFocus);
    //---添加表格---
    QStringList headerLabels;
    int rows,columns;
    rows=10;
    columns=5;
    headerLabels<< "项目名称"<< "分摊时间"<< "分摊原因"<< "分摊费用"<<"分摊人员";
    //创建表格
    m_table=new QTableWidget(rows,columns);
    m_table->setMaximumHeight(497);
    m_table->setMinimumHeight(497);
    //行标题
    m_table->setHorizontalHeaderLabels(headerLabels);
    m_table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_table->verticalHeader()->setDefaultSectionSize(47);//行高
    m_table->horizontalHeader()->setDefaultSectionSize(100);//列宽
    m_table->setColumnWidth(0,120);
    //设置对齐
    m_table->horizontalHeader()->setStretchLastSection(true);//右边界对齐
    m_table->verticalHeader()->setStretchLastSection(true);//下边界对齐
    //设置方式
    m_table->setSelectionBehavior ( QAbstractItemView::SelectRows);//设置表格的选择方式
    m_table->setEditTriggers(false);//设置编辑方式
    m_table->setFocusPolicy(Qt::ClickFocus);
    connect(m_table,&QTableWidget::doubleClicked,this,&ErrorWidget::ButtonDetailsPress);
    QObject::connect(m_table,&QTableWidget::itemSelectionChanged,[=](){
        m_btn3->setEnabled(true);
        m_btn4->setEnabled(true);
    });
    //---布局---
    m_layout->addStretch(1);
    m_layout->addLayout(m_findLayout);
    m_layout->addLayout(m_btnLayout);
    m_layout->addStretch(1);
    m_layout->addWidget(m_table);

    this->setLayout(m_layout);
    m_parent->setSize(1100,650);
}

void ErrorWidget::clearTable(){//重置
    int count;
    while((count=m_table->rowCount())>0)
        m_table->removeRow(count-1);
    for(int i=0;i<10;i++)
        m_table->insertRow(0);
    m_result->setText("");
    if(m_dataList!=NULL){
        foreach(ErrorData* data,(*m_dataList))
            delete data;
        delete m_dataList;
        m_dataList=NULL;
    }
}

void ErrorWidget::ItemDetailsFindFinished(QList<ErrorData *>* dataList){//外部强制查询-结束槽函数
    disconnect(m_data, &Data::SendErrorData, this, &ErrorWidget::ItemDetailsFindFinished);
    if(dataList==NULL){
        return;
    }
    m_out=true;
    //内容
    ErrorData *t_data;
    t_data = dataList->first();
    dataList->clear();
    delete dataList;
    m_detailsWidget->reWidget(t_data);
    m_detailsWidget->exec();
    m_out=false;
}

void ErrorWidget::ItemDetailsFind(const QString ID){//外部强制查询
    //寻找
    connect(m_data, &Data::SendErrorData, this, &ErrorWidget::ItemDetailsFindFinished,Qt::QueuedConnection);
    m_data->GetDatabase("errorDatabase.db","ALL",QString(" WHERE ID='%1'").arg(ID));
}

void ErrorWidget::ButtonFindPressFinished(QList<ErrorData *> *dataList){//获取数据到表格-结束槽函数
    disconnect(m_data, &Data::SendErrorData, this, &ErrorWidget::ButtonFindPressFinished);
    m_dataList=dataList;
    m_btn3->setEnabled(false);
    m_btn4->setEnabled(false);
    m_btn5->setEnabled(false);
    if(dataList==NULL){
        delete dataList;
        return;
    }
    int i,j;
    QStringList *strList;
    strList=&Type::Error;
    //打印按钮
    m_btn5->setEnabled(true);
    //开始添加
    i=0;
    int count=0;
    double price=0;
    foreach(ErrorData *temp,(*dataList)){
        j=0;
        if(m_table->rowCount()<=i){
            m_table->insertRow(i);
            m_table->setRowHeight(i,47);
        }
        foreach(QString str,(*strList)){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            item->setText(temp->GetData("show",str));
            m_table->setItem(i,j,item);
            j++;
        }
        price+=temp->price;
        i++;
    }
    //结果
    count=i;
    m_result->setText(QString("搜索结果：%1条\t\t\t\t总分摊费用：%2元").arg(count).arg(QString::number(price,'d',2)));
}

void ErrorWidget::ButtonFindPress(){//查询按钮
    clearTable();
    connect(m_data, &Data::SendErrorData, this, &ErrorWidget::ButtonFindPressFinished,Qt::QueuedConnection);

    QString str1,str2;
    int count=0;
    //分摊时间
    str1=QString(" WHERE %1>='%2' and %1<='%3'").arg("date").arg(m_dateSBox->GetData()).arg(m_dateEBox->GetData());
    count++;
    //费用
    if(m_priceBox->GetFrom()!=-1 && count>0)
        str1+=QString(" and ");
    if(m_priceBox->GetFrom()!=-1){
        str1+=QString("(%1>=%2 and %1<=%3)").arg("price").arg(m_priceBox->GetFrom()).arg(m_priceBox->GetTo());
        count++;
    }
    //搜索框
    str2 = m_edit->text();
    if(str2!="" && count>0)
        str1+=QString(" and ");
    if(str2.length()!=0){
        str1+=QString("%1 like '%%%4%%' or %2 like '%%%4%%' or %3 like '%%%4%%'").arg("name").arg("reason").arg("list").arg(str2);
        count++;
    }
    //结尾
    if(str1==" WHERE")
        str1="";
    if(m_dateSBox->GetData().left(4)==m_dateEBox->GetData().left(4))
        str2=m_dateSBox->GetTable();
    else
        str2="ALL";
    m_parent->m_loading->Start("GetDatabase","errorDatabase.db",str2,str1);
}
//添加
void ErrorWidget::ButtonAddPress(){
    m_addWidget->reWidget();
    m_addWidget->exec();
}

void ErrorWidget::ButtonAddPressEnter (){
    //判断
    if(m_addWidget->m_name->GetValue().length()==0){
        m_parent->MessageHit("项目名称不能为空");
        return;
    }
    if(m_addWidget->m_price->GetValue().toLongLong()==0){
        m_parent->MessageHit("金额不能为0");
        return;
    }
    if(m_addWidget->m_itemList->count()==0){
        m_parent->MessageHit("员工列表不能为空");
        return;
    }
    if(m_addWidget->m_itemBtn1->isEnabled()){
        m_parent->MessageHit("员工分摊不足100%");
        return;
    }
    //新建
    QString str;
    ErrorData *data=new ErrorData();
    data->name=m_addWidget->m_name->GetValue();
    data->date=m_addWidget->m_dateBox->GetData();
    data->price=m_addWidget->m_price->GetValue().toDouble();
    data->reason=m_addWidget->m_reason->GetValue();
    data->ID=m_data->GetNextID("errorDatabase.db","ID",data->date.left(8));
    data->unSale="00";
    if(data->ID=="RangeUp"){
        m_parent->MessageHit("当天分摊项目超过9999次");
        delete data;
        return;
    }
    for(int i=0;i<m_addWidget->m_itemList->count();i++){
        str=m_addWidget->m_itemList->item(i)->text();
        data->staffIDname.append(str.section("  ",0,0));
        data->staffPrice.append(str.section("  ",2,2).remove(0,1).toDouble());
    }
    //提示框
    str=QString("是否添加一项分摊费用数据？\n项目名称：%1\n分摊时间：%2\n总金额：%3\n人员：%4")
                        .arg(data->GetData("show","name")).arg(data->GetData("show","date")).arg(data->GetData("show","price")).arg(data->GetData("show","list"));
    if(data->reason!="")
        str+=QString("\n分摊原因：%1").arg(data->reason);
    if(m_parent->MessageQuestion(str)){
        //添加
        if(m_data->InsertDatabase(data)){
            m_addWidget->close();
            m_parent->errorEnd();
            m_parent->MessageHit(QString("添加成功!"));
            ButtonFindPress();
        }
        else
            m_parent->MessageHit(QString("添加失败!"));
    }
    delete data;
}

//修改
void ErrorWidget::ButtonEditPress(){
    //获取选中
    int count=m_table->selectedItems().count()/5;
    if(count==0)
        return;
    if(count>1){
        m_parent->MessageHit("不能批量修改");
        return;
    }
    int index=m_table->selectedRanges().first().topRow();
    if(m_dataList->at(index)->unSale!="00"){
        if(m_dataList->at(index)->unSale=="01")
            m_parent->MessageHit("已结算不能修改!");
        else
            m_parent->MessageHit("已取消不能修改!");
        return;
    }

    //内容
    m_editWidget->reWidget(m_dataList->at(index));
    m_editWidget->exec();
}

void ErrorWidget::ButtonEditPressEnter (){
    //判断
    if(m_editWidget->m_name->GetValue().length()==0){
        m_parent->MessageHit("项目名称不能为空");
        return;
    }
    if(m_editWidget->m_price->GetValue().toLongLong()==0){
        m_parent->MessageHit("金额不能为0");
        return;
    }
    if(m_editWidget->m_itemList->count()==0){
        m_parent->MessageHit("员工列表不能为空");
        return;
    }
    if(m_editWidget->m_itemBtn1->isEnabled()){
        m_parent->MessageHit("员工分摊不足100%");
        return;
    }
    //新建
    QString str;
    ErrorData *error=new ErrorData();
    error->name=m_editWidget->m_name->GetValue();
    error->date=m_editWidget->m_dateBox->GetData();
    error->price=m_editWidget->m_price->GetValue().toDouble();
    error->reason=m_editWidget->m_reason->GetValue();
    QListWidget *list=m_editWidget->m_itemList;
    error->staffIDname.clear();
    error->staffPrice.clear();
    error->ID=m_editWidget->m_record->ID;
    error->unSale="00";
    for(int i=0;i<list->count();i++){
        str=list->item(i)->text();
        error->staffIDname.append(str.section("  ",0,0));
        error->staffPrice.append(str.section("  ",2,2).remove(0,1).toDouble());
    }
    //提示框
    QStringList strList;
    strList.clear();
    ErrorData *data=m_editWidget->m_record;
    str="";
    if(data->name!=error->name){
        strList.append("name");
        str=QString("\n名称: %1->%2").arg(data->name).arg(error->name);
    }
    if(data->date!=error->date){
        strList.append("date");
        str+=QString("\n时间: %1->%2").arg(data->GetData("show","date")).arg(error->GetData("show","date"));
    }
    if(data->price!=error->price){
        strList.append("price");
        str+=QString("\n金额: %1->%2").arg(data->GetData("show","price")).arg(error->GetData("show","price"));
    }
    if(data->reason!=error->reason){
        QString tempstr;
        strList.append("reason");
        tempstr=data->reason;
        if(tempstr=="")
            tempstr="空";
        str+=QString("\n原因      : %1").arg(tempstr);
        tempstr=error->reason;
        if(tempstr=="")
            tempstr="空";
        str+=QString("->%1").arg(tempstr);
    }
    if(data->GetData("write","list")!=error->GetData("write","list")){
        strList.append("list");
        str+=QString("\n员工: %1->%2").arg(data->GetData("show","list")).arg(error->GetData("show","list"));
    }

    if(str.length()==0){
        delete error;
        m_parent->MessageHit("未修改任何数据!");
        return;
    }

    str=QString("是否修改一项分摊项目数据？")+str;
    //修改
    if(m_parent->MessageQuestion(str)){
        m_editWidget->close();
        //修改固定资产
        if(!m_data->Update(data->ID,strList,error)){
            m_parent->MessageHit("修改失败!");
            delete error;
            return;
        }
        //清界面
        for(int i=m_table->rowCount()-1;i>=0;i--){
            if(m_table->item(i,0)!=NULL && m_dataList->count()>i)
                if(m_dataList->at(i)->ID==data->ID){//寻找
                    foreach(QString str,strList){
                        if(str=="name"){
                            m_table->item(i,0)->setText(error->GetData("show","name"));
                            data->name=error->name;
                        }
                        else if(str=="date"){
                            m_table->item(i,1)->setText(error->GetData("show","date"));
                            data->date=error->date;
                            data->ID=error->ID;
                        }
                        else if(str=="reason"){
                            m_table->item(i,2)->setText(error->GetData("show","reason"));
                            data->reason=error->reason;
                        }
                        else if(str=="price"){
                            m_table->item(i,3)->setText(error->GetData("show","price"));
                            data->price=error->price;
                        }
                        else if(str=="list"){
                            m_table->item(i,4)->setText(error->GetData("show","list"));
                            data->staffIDname=error->staffIDname;
                            data->staffPrice=error->staffPrice;
                        }
                    }
                }
        }
        //成功
        m_parent->MessageHit("修改成功!");
    }
    delete error;
}

void ErrorWidget::ButtonDelPress(){//删除按钮
    //获取选中
    int count=m_table->selectedItems().count()/5;
    if(count==0)
        return;
    m_IDList.clear();
    m_selectionRange=m_table->selectedRanges();
    //记录串码
    m_IDList.clear();
    foreach(QTableWidgetSelectionRange range,m_selectionRange)
        for(int i=range.topRow();i<=range.bottomRow();i++){
            if(m_table->item(i,0)==NULL)
                break;
            m_IDList.append(m_dataList->at(i)->ID);
        }
    //询问是否删除
    QString str;
    str=QString("是否删除%1条分摊记录(员工的业绩不会消除)").arg(count);
    if(count==1)
        str+=QString("\n第%1行").arg(m_selectionRange.first().topRow()+1);
    else foreach(QTableWidgetSelectionRange range,m_selectionRange)
        if(range.rowCount()==1)
            str+=QString("\n第%1行").arg(range.topRow()+1);
        else
            str+=QString("\n第%1行到第%2行").arg(range.topRow()+1).arg(range.bottomRow()+1);
    str+="?";
    //删除
    if(m_parent->MessageQuestion(str)){
        if(!m_parent->MessageQuestion("请再次确认"))
            return;
        disconnect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        if(m_data->DeleteDatabase("errorDatabase.db","ID",m_IDList))
            m_parent->MessageHit("删除成功!");
        else
            m_parent->MessageHit("删除失败!");
        this->ButtonFindPress();
        connect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        return;
    }
}

void ErrorWidget::ButtonDelAllPress(){//清空
    m_table->selectAll();
    //获取选中
    int count=m_table->selectedItems().count()/5;
    if(count==0)
        return;
    m_selectionRange=m_table->selectedRanges();
    //记录串码
    m_IDList.clear();
    foreach(QTableWidgetSelectionRange range,m_selectionRange)
        for(int i=range.topRow();i<=range.bottomRow();i++){
            if(m_table->item(i,0)==NULL)
                break;
            m_IDList.append(m_dataList->at(i)->ID);
        }
    //询问是否删除
    QString str;
    str=QString("是否清空当前%1个数据？(员工的业绩不会消除)").arg(count);
    //删除
    if(m_parent->MessageQuestion(str)){
        if(!m_parent->MessageQuestion("请再次确认"))
            return;
        disconnect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        if(m_data->DeleteDatabase("errorDatabase.db","ID",m_IDList))
            m_parent->MessageHit("删除成功!");
        else
            m_parent->MessageHit("删除失败!");
        this->ButtonFindPress();
        connect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        return;
    }
}

void ErrorWidget::ButtonDetailsPress(){//详情
    //获取选中
    int count=m_table->selectedItems().count()/5;
    if(count!=1)
        return;
    int index=m_table->selectedRanges().first().topRow();
    //内容
    m_detailsWidget->reWidget(m_dataList->at(index));
    m_detailsWidget->exec();
}
void ErrorWidget::DetailsWidgetUnsale(){//取消分摊
    if(m_data->UnSaleBasedata(m_detailsWidget->m_record)){
        if(!m_out){
            //获取选中
            int index=m_table->selectedRanges().first().topRow();
            //清本界面
            m_table->item(index,1)->setText("已取消");
        }
        m_detailsWidget->close();
        //清其他界面
        m_parent->errorEnd();
        m_parent->MessageHit("取消成功！");
    }
    else
        m_parent->MessageHit("取消失败！");
}

/*----键盘响应----*/
void ErrorWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return))
        m_btn1->click();
}
