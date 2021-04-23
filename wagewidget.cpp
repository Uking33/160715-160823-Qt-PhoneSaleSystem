//窗口
#include "wagewidget.h"
#include "wageitemwidget.h"
#include "mainwindow.h"
#include "uloading.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
//控件
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include "ubutton.h"
#include "uline.h"
#include "urectbox.h"
#include "udatebox.h"
//数据
#include "data.h"
//辅助
#include <QHeaderView>
#include <QTableWidgetSelectionRange>
#include <QKeyEvent>
#include <QCheckBox>
#include <QComboBox>

WageWidget::WageWidget(MainWindow *parent, Data *data)
    : QWidget(parent)
{
    //初始化
    m_parent=parent;
    m_data=data;
    m_dataList=NULL;
    createWidget();
    //子界面
    m_addWidget = new WageItemWidget(this,"ADD");
    m_detailsWidget = new WageItemWidget(this,"DETAILS");
}

WageWidget::~WageWidget(){
    delete m_layout;
}

void WageWidget::createWidget(){//创建
    m_layout = new QVBoxLayout();
    //---限定---
    m_findLayout = new QHBoxLayout();
    //起始时间
    m_dateSLabel = new QLabel("首日起始");
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
    m_dateELabel = new QLabel("首日结束");
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
    m_priceBox = new URectBox("工资","元","元");
    m_priceBox->m_checkBox->setShortcut(QKeySequence("F3"));
    //资产名称
    m_findLabel = new QLabel("搜索:");
    m_edit = new QLineEdit();
    m_edit->setFocusPolicy(Qt::StrongFocus);
    m_edit->setPlaceholderText("员工/增减项名称");
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
    m_btn3 = new UButton("删除(Delete)","Delete",80,30,m_btnLayout);
    m_btnLayout->addStretch(1);
    m_btn4 = new UButton("清空(Esc)","Esc",80,30,m_btnLayout);
    m_btnLayout->addStretch(1);
    m_btn3->setEnabled(false);
    m_btn4->setEnabled(false);
    //信号连接
    connect(m_btn1,SIGNAL(clicked()),this,SLOT(ButtonFindPress()));
    connect(m_btn2,SIGNAL(clicked()),this,SLOT(ButtonAddPress()));
    connect(m_btn3,SIGNAL(clicked()),this,SLOT(ButtonDelPress()));
    connect(m_btn4,SIGNAL(clicked()),this,SLOT(ButtonDelAllPress()));
    m_btn1->setFocusPolicy(Qt::NoFocus);
    m_btn2->setFocusPolicy(Qt::NoFocus);
    m_btn3->setFocusPolicy(Qt::NoFocus);
    m_btn4->setFocusPolicy(Qt::NoFocus);
    //---添加表格---
    QStringList headerLabels;
    int rows,columns;
    rows=10;
    columns=7;
    headerLabels<< "员工"<< "日期"<< "工资"<< "底薪"<<"毛利"<<"提成"<<"增减项";
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
    connect(m_table,&QTableWidget::doubleClicked,this,&WageWidget::ButtonDetailsPress);
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

void WageWidget::clearTable(){//重置
    int count;
    while((count=m_table->rowCount())>0)
        m_table->removeRow(count-1);
    for(int i=0;i<10;i++)
        m_table->insertRow(0);
    m_result->setText("");
    if(m_dataList!=NULL){
        foreach(WageData* data,(*m_dataList))
            delete data;
        delete m_dataList;
        m_dataList=NULL;
    }
}

void WageWidget::ButtonFindPressFinished(QList<WageData *> *dataList){//获取数据到表格-结束槽函数
    disconnect(m_data, &Data::SendWageData, this, &WageWidget::ButtonFindPressFinished);
    m_dataList=dataList;
    m_btn3->setEnabled(false);
    m_btn4->setEnabled(false);
    if(dataList==NULL){
        delete dataList;
        return;
    }
    int i,j;
    QStringList *strList;
    strList=&Type::WageShow;
    //清空按钮
    m_btn4->setEnabled(true);
    //开始添加
    i=0;
    int count=0;
    double price=0;
    foreach(WageData *temp,(*dataList)){
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
        price+=temp->wage;
        i++;
    }
    //结果
    count=i;
    m_result->setText(QString("搜索结果：%1条\t\t\t\t总工资：%2元").arg(count).arg(QString::number(price,'d',2)));
}

void WageWidget::ButtonFindPress(){//查询按钮
    clearTable();
    connect(m_data, &Data::SendWageData, this, &WageWidget::ButtonFindPressFinished,Qt::QueuedConnection);

    QString str1,str2;
    int count=0;
    //起始时间
    str1=QString(" WHERE %1>='%2' and %1<='%3'").arg("startDate").arg(m_dateSBox->GetData()).arg(m_dateEBox->GetData());
    count++;
    //费用
    if(m_priceBox->GetFrom()!=-1 && count>0)
        str1+=QString(" and ");
    if(m_priceBox->GetFrom()!=-1){
        str1+=QString("(%1>=%2 and %1<=%3)").arg("wage").arg(m_priceBox->GetFrom()).arg(m_priceBox->GetTo());
        count++;
    }
    //搜索框
    str2 = m_edit->text();
    if(str2!="" && count>0)
        str1+=QString(" and ");
    if(str2.length()!=0){
        str1+=QString("%1 like '%%%3%%' or %2 like '%%%3%%'").arg("IDname").arg("name").arg(str2);
        count++;
    }
    //结尾
    if(str1==" WHERE")
        str1="";
    if(m_dateSBox->GetData().left(4)==m_dateEBox->GetData().left(4))
        str2=m_dateSBox->GetTable();
    else
        str2="ALL";
    m_parent->m_loading->Start("GetDatabase","wageDatabase.db",str2,str1);
}

//添加
void WageWidget::ButtonAddPress(){
    m_addWidget->reWidget();
    if(m_addWidget->m_nameBox->count()==0)
        m_parent->MessageHit("请添加员工后再结算工资！");
    else
        m_addWidget->exec();
}

void WageWidget::ButtonAddPressEnter (QList<StaffDataList *>*list){
    disconnect(m_data, &Data::SendStaffData, this, &WageWidget::ButtonAddPressEnter);
    StaffDataList *staff=list->first();
    //新建
    QString str1,str2;
    double sum=0;
    WageData *data=new WageData();
    data->IDname=m_addWidget->m_nameBox->currentText();
    data->startDate=m_addWidget->m_stratDateBox->GetData();
    data->endDate=m_addWidget->m_endDateBox->GetData();
    data->per=m_addWidget->m_percent->GetValue().toDouble();
    data->base=m_addWidget->m_base->GetValue().toDouble();
    data->gather=0;
    //rec
    if(staff->recList!=NULL && staff->recList->count()>0)
        foreach(StaffRec* rec,(*staff->recList)){
            if(!rec->work.contains("营业员"))
                continue;
            if(rec->unSale=="00"){
                data->gather+=rec->gather;
                rec->unSale="01";
            }
            else if(rec->unSale=="10")
                rec->unSale="11";
            else if(rec->unSale=="21"){
                data->gather-=rec->gather;
                rec->unSale=data->startDate+data->endDate;
            }
        }
    for(int i=0;i<m_addWidget->m_itemList->count();i++){
        str1=m_addWidget->m_itemList->item(i)->text();
        RewordData reword;
        reword.name=str1.section(" ",0,0);
        str2=str1.section(" ",1,1);
        reword.price=str2.remove(str2.length()-1,1).toDouble();
        sum+=reword.price;
        reword.reason=str1.section(" ",2,2);
        reword.errorID="";
        reword.date="";
        data->rewardList.append(reword);
    }
    //err
    if(staff->errorList!=NULL && staff->errorList->count()>0)
        foreach(StaffError* error,(*staff->errorList)){
            RewordData reword;
            reword.errorID=error->ID;
            reword.date=error->date;
            reword.name=error->name;
            if(error->unSale=="21")
                reword.price=-error->price;
            else
                reword.price=error->price;
            sum+=reword.price;
            reword.reason=error->reason;
            data->rewardList.append(reword);
        }
    if(staff->errorList!=NULL && staff->errorList->count()>0)
        foreach(StaffError* error,(*staff->errorList))
            if(error->unSale=="00")
                error->unSale="01";
            else if(error->unSale=="10")
                error->unSale="11";
            else if(error->unSale=="21")
                error->unSale=data->startDate+data->endDate;
    //wage
    data->wage=data->base+data->gather*data->per/100+sum;
    if(data->wage==0 && data->gather==0 && staff->errorList->count()==0&& staff->recList->count()==0){
        foreach(StaffDataList* d,(*list))
            delete d;
        delete list;
        delete data;
        m_parent->MessageHit("结算失败!\n无任何这个员工在这时间段的记录且工资为0元");
        return;
    }
    //提示框
    str1=QString("是否结算%1的%2到%3工资？\n工资:%4\n底薪:%5\n利润:%6\n提成:%7\n")
            .arg(data->GetData("show","name")).arg(data->GetData("show","startDate")).arg(data->GetData("show","endDate"))
            .arg(data->GetData("show","wage")).arg(data->GetData("show","base")).arg(data->GetData("show","gather"))
            .arg(data->GetData("show","per"));
    if((str2=data->GetData("show","list"))!="")
        str1+="奖罚:"+str2+"\n";
    else
        str1+="奖罚:空\n";
    if((str2=data->GetData("show","error"))!="")
        str1+="分摊:"+str2+"\n";
    else
        str1+="分摊:空\n";

    if(m_parent->MessageQuestion(str1)){
        //添加
        if(m_data->InsertDatabase(data,staff->recList,staff->errorList)){
            m_addWidget->close();
            m_parent->MessageHit(QString("结算成功!"));
            ButtonFindPress();
        }
        else
            m_parent->MessageHit(QString("结算失败!"));
    }
    foreach(StaffDataList* d,(*list))
        delete d;
    delete list;
    delete data;
}

void WageWidget::ButtonDelPress(){//删除按钮
    //获取选中
    int count=m_table->selectedItems().count()/7;
    if(count==0)
        return;
    m_IDList.clear();
    m_selectionRange=m_table->selectedRanges();
    //记录串码+
    QStringList strList;
    m_IDList.clear();
    foreach(QTableWidgetSelectionRange range,m_selectionRange)
        for(int i=range.topRow();i<=range.bottomRow();i++){
            if(m_table->item(i,0)==NULL)
                break;
            m_IDList.append(m_dataList->at(i)->IDname);
            strList.append(m_dataList->at(i)->startDate);
        }
    //询问是否删除
    QString str;
    str=QString("是否删除%1条工资记录(不会影响业绩，已经结算的不能再次结算)").arg(count);
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
        m_parent->m_loading->Start("DeleteDatabase","wageDatabase.db",m_IDList,strList);
        //if(m_data->DeleteDatabase("wageDatabase.db",m_IDList,strList))
        m_parent->MessageHit("删除成功!");
        //else
            //m_parent->MessageHit("删除失败!");
        this->ButtonFindPress();
        connect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        return;
    }
}

void WageWidget::ButtonDelAllPress(){//清空
    m_table->selectAll();
    //获取选中
    int count=m_table->selectedItems().count()/7;
    if(count==0)
        return;
    count=0;
    m_selectionRange=m_table->selectedRanges();
    //记录串码
    QStringList strList;
    m_IDList.clear();
    foreach(QTableWidgetSelectionRange range,m_selectionRange)
        for(int i=range.topRow();i<=range.bottomRow();i++){
            if(m_table->item(i,0)==NULL)
                break;
            m_IDList.append(m_dataList->at(i)->IDname);
            strList.append(m_dataList->at(i)->startDate);
            count++;
        }
    //询问是否删除
    QString str;
    str=QString("是否删除当前搜索的%1条工资记录(不会影响业绩，已经结算的不能再次结算)？").arg(count);
    //删除
    if(m_parent->MessageQuestion(str)){
        if(!m_parent->MessageQuestion("请再次确认"))
            return;
        disconnect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        m_parent->m_loading->Start("DeleteDatabase","wageDatabase.db",m_IDList,strList);
        //if(m_data->DeleteDatabase("wageDatabase.db",m_IDList,strList))
        m_parent->MessageHit("删除成功!");
        //else
            //m_parent->MessageHit("删除失败!");
        this->ButtonFindPress();
        connect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        return;
    }
}

void WageWidget::ButtonDetailsPress(){//详情
    //获取选中
    int count=m_table->selectedItems().count()/7;
    if(count!=1)
        return;
    int index=m_table->selectedRanges().first().topRow();
    //内容
    m_detailsWidget->reWidget(m_dataList->at(index));
    m_detailsWidget->exec();
}

/*----键盘响应----*/
void WageWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return))
        m_btn1->click();
}
