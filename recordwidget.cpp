//窗口
#include "recordwidget.h"
#include "recorditemwidget.h"
#include "mainwindow.h"
#include "printsum.h"
#include "uloading.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
//控件
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include "ubutton.h"
#include "udatebox.h"
#include "urectbox.h"
//数据
#include "data.h"
//辅助
#include <QKeyEvent>
#include <QHeaderView>
#include <QTableWidgetSelectionRange>

RecordWidget::RecordWidget(MainWindow *parent, Data *data){
    //初始化
    m_parent=parent;
    m_data=data;
    m_dataList=NULL;
    m_out=false;
    createWidget();
    //详情界面
    m_itemWidget = new RecordItemWidget(this);
}

RecordWidget::~RecordWidget(){
    clearTable();
    delete m_layout;
}

void RecordWidget::createWidget(){//创建
    m_layout = new QVBoxLayout();
    //---限定---
    m_findLayout = new QHBoxLayout();
    //开单时间
    m_outdateSLabel = new QLabel("开单起始");
    m_outdateSBox = new UDateBox(2);
    m_outdateSLayout= new QHBoxLayout();
    m_outdateSLayout->addWidget(m_outdateSLabel);
    m_outdateSLayout->addWidget(m_outdateSBox);
    m_outdateSBtn1 = new UButton("日","F1",25,25,m_outdateSLayout);
    m_outdateSBtn1->setFocusPolicy(Qt::NoFocus);
    connect(m_outdateSBtn1,&UButton::clicked,m_outdateSBox,&UDateBox::Today);
    m_outdateSBtn2 = new UButton("周","F2",25,25,m_outdateSLayout);
    m_outdateSBtn2->setFocusPolicy(Qt::NoFocus);
    connect(m_outdateSBtn2,&UButton::clicked,m_outdateSBox,&UDateBox::ToWeek);
    m_outdateSLayout->setSpacing(5);
    //开单时间
    m_outdateELabel = new QLabel("开单结束");
    m_outdateEBox = new UDateBox(3,m_outdateSBox);
    m_outdateSBox->ChooseDay(m_outdateSBox->m_comboBoxDay->currentText());
    m_outdateELayout= new QHBoxLayout;
    m_outdateELayout->addWidget(m_outdateELabel);
    m_outdateELayout->addWidget(m_outdateEBox);
    m_outdateEBtn1 = new UButton("月","F5",25,25,m_outdateELayout);
    m_outdateEBtn1->setFocusPolicy(Qt::NoFocus);
    connect(m_outdateEBtn1,&UButton::clicked,m_outdateSBox,&UDateBox::ToMonth);
    m_outdateEBtn2 = new UButton("年","F6",25,25,m_outdateELayout);
    m_outdateEBtn2->setFocusPolicy(Qt::NoFocus);
    connect(m_outdateEBtn2,&UButton::clicked,m_outdateSBox,&UDateBox::ToYear);
    m_outdateELayout->setSpacing(5);
    //售价
    m_saleBox = new URectBox("售价","元","元");
    m_saleBox->m_checkBox->setShortcut(QKeySequence("F3"));
    //单号-营业员-收银员-串码
    m_findLabel = new QLabel("搜索:");
    m_find = new QLineEdit();
    m_find->setFocusPolicy(Qt::StrongFocus);
    m_find->setPlaceholderText("单号/串码/物体名称/营业员/收银员");
    m_find->setMaxLength(50);
    m_find->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9\u4e00-\u9fa5]*$"), m_find));
    //毛利
    m_gatherBox = new URectBox("毛利","元","元");
    m_gatherBox->m_checkBox->setShortcut(QKeySequence("F4"));
    //布局
    m_findLayout->addLayout(m_outdateSLayout);
    m_findLayout->addWidget(m_saleBox);
    m_findLayout->addWidget(m_gatherBox);
    m_findLayout->addWidget(m_findLabel);
    m_findLayout->addWidget(m_find);
    //---结果---
    m_result = new QLabel();
    m_result->setAlignment(Qt::AlignCenter);
    //---按钮---
    m_btnLayout = new QHBoxLayout();
    m_btnLayout->addLayout(m_outdateELayout);
    m_btnLayout->addWidget(m_result,2);
    //创建按钮
    m_btn1 = new UButton("查询(Enter)","",80,30,m_btnLayout);
    m_btn2 = new UButton("删除(Delete)","Delete",80,30,m_btnLayout);
    m_btn3 = new UButton("清空(Esc)","Esc",80,30,m_btnLayout);
    m_btn2->setEnabled(false);
    m_btn3->setEnabled(false);
    //信号连接
    connect(m_btn1,SIGNAL(clicked()),this,SLOT(ButtonFindPress()));
    connect(m_btn2,SIGNAL(clicked()),this,SLOT(ButtonDelPress()));
    connect(m_btn3,SIGNAL(clicked()),this,SLOT(ButtonDelAllPress()));
    m_btn1->setFocusPolicy(Qt::NoFocus);
    m_btn2->setFocusPolicy(Qt::NoFocus);
    m_btn3->setFocusPolicy(Qt::NoFocus);
    //---添加表格---
    QStringList headerLabels;    
    m_col=7;
    headerLabels<< "单号"<< "开单时间"<< "营 业 员"<< "收 银 员"<< "售价"<< "毛利"<< "项目";
    //创建表格
    m_table=new QTableWidget(10,m_col);
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
    connect(m_table,&QTableWidget::doubleClicked,this,&RecordWidget::ButtonDetailsPress);
    QObject::connect(m_table,&QTableWidget::itemSelectionChanged,[=](){
        m_btn2->setEnabled(true);
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

void RecordWidget::clearTable(){//重置
    int count;
    while((count=m_table->rowCount())>0)
        m_table->removeRow(count-1);
    for(int i=0;i<10;i++)
        m_table->insertRow(0);
    m_result->setText("");
    //m_dataList
    if(m_dataList!=NULL){
        foreach(PhoneRecordList* data,(*m_dataList))
            delete data;
        delete m_dataList;
        m_dataList=NULL;
    }
}

void RecordWidget::ItemDetailsFindFinished(QList<PhoneRecordList *>* dataList){//外部强制查询-结束槽函数
    disconnect(m_data, &Data::SendRecordData, this, &RecordWidget::ItemDetailsFindFinished);
    if(dataList==NULL){
        return;
    }
    m_out=true;
    //内容
    PhoneRecordList *t_data;
    t_data = dataList->first();
    dataList->clear();
    delete dataList;
    m_itemWidget->reWidget(t_data);
    m_itemWidget->exec();
    m_out=false;
}

void RecordWidget::ItemDetailsFind(const QString ID){//外部强制查询
    //寻找
    connect(m_data, &Data::SendRecordData, this, &RecordWidget::ItemDetailsFindFinished,Qt::QueuedConnection);
    m_data->GetDatabase("recordDatabase.db","ALL",QString(" WHERE ID='%1'").arg(ID));
}

void RecordWidget::ButtonFindPressFinished(QList<PhoneRecordList *> *dataList){//获取数据到表格-结束槽函数
    disconnect(m_data, &Data::SendRecordData, this, &RecordWidget::ButtonFindPressFinished);
    m_btn2->setEnabled(false);
    m_btn3->setEnabled(false);
    if(dataList==NULL){
        return;
    }
    m_dataList=dataList;
    int i,j;
    QStringList *strList;
    strList=&Type::RECORD;
    //打印按钮
    m_btn3->setEnabled(true);
    //开始添加
    i=0;
    int count=0;
    double price=0,gather=0;
    foreach(PhoneRecordList *temp,(*dataList)){
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
        if(m_table->item(i,1)->text()!="已退货"){
            price+=temp->price;
            gather+=temp->gather;
        }
        else
            count++;
        i++;
    }
    //结果
    m_result->setText(QString("搜索结果：%1条\t\t退货：%2条\t\t总售价：%3元\t\t总毛利：%4元")
                      .arg(i).arg(count).arg(QString::number(price,'d',2)).arg(QString::number(gather,'d',2)));
}

void RecordWidget::ButtonFindPress(){//查询按钮
    clearTable();
    connect(m_data, &Data::SendRecordData, this, &RecordWidget::ButtonFindPressFinished,Qt::QueuedConnection);

    QString str1,str2;
    int count=0;
    //开单时间
    str1=QString(" WHERE %1>='%2' and %1<='%3'").arg("outTime").arg(m_outdateSBox->GetData()+"000000").arg(m_outdateEBox->GetData()+"235959");
    count++;
    //售价
    if(m_saleBox->GetFrom()!=-1 && count>0)
        str1+=QString(" and ");
    if(m_saleBox->GetFrom()!=-1){
        str1+=QString("(%1>=%2 and %1<=%3)").arg("price").arg(m_saleBox->GetFrom()).arg(m_saleBox->GetTo());
        count++;
    }
    //毛利
    if(m_gatherBox->GetFrom()!=-1 && count>0)
        str1+=QString(" and ");
    if(m_gatherBox->GetFrom()!=-1){
        str1+=QString("(%1>=%2 and %1<=%3)").arg("gather").arg(m_gatherBox->GetFrom()).arg(m_gatherBox->GetTo());
        count++;
    }
    //搜索框
    str2 = m_find->text();
    str2=str2.simplified();
    if(str2=="")
        m_find->setText("");
    else if(count>0)
        str1+=QString(" and ");
    if(str2.length()==12 && str2.toLongLong()!=0){//单号
        str1+=QString("%1 like '%%%2%%'").arg("ID").arg(str2);
        count++;
    }
    else if(str2.length()!=12 && str2!=""){
        if(str2.length()<12 && str2.toLongLong()!=0)
            str1+=QString("(%1 like '%%%2%%' or").arg("ID").arg(str2);
        else
            str1+=QString("(");
        //销售/营业员/串码/名称
        str1+=QString("(shopAssistant like '%%%1%%' or cashier like '%%%1%%' or phoneModels like '%%%1%%' or IMEI like '%%%1%%'))").arg(str2);
        count++;
    }
    //结尾
    if(m_outdateSBox->GetData().left(4)==m_outdateEBox->GetData().left(4))
        str2=m_outdateSBox->GetTable();
    else
        str2="ALL";
    if(str1==" WHERE")
        str1="";
    m_parent->m_loading->Start("GetDatabase","recordDatabase.db",str2,str1);
}

void RecordWidget::ButtonDelPress(){//删除按钮
    //获取选中
    int count=m_table->selectedItems().count()/m_col;;
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
            m_IDList.append(m_table->item(i,0)->text());
        }
    //询问是否删除
    QString str;
    str=QString("是否删除%1条销售记录(员工的业绩不会消除)").arg(count);
    if(count==1)
        str+=QString("\n第%1行:%2").arg(m_selectionRange.first().topRow()+1).arg(m_IDList.first());
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
        if(m_data->DeleteDatabase("recordDatabase.db","ID",m_IDList))
            m_parent->MessageHit("删除成功!");
        else
            m_parent->MessageHit("删除失败!");
        this->ButtonFindPress();
        connect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        return;
    }
}

void RecordWidget::ButtonDelAllPress(){//清空
    m_table->selectAll();
    //获取选中
    int count=m_table->selectedItems().count()/m_col;
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
            m_IDList.append(m_table->item(i,0)->text());
        }
    //询问是否删除
    QString str;
    str=QString("是否清空当前%1条销售记录(员工的业绩不会消除)？").arg(count);
    //删除
    if(m_parent->MessageQuestion(str)){
        if(!m_parent->MessageQuestion("请再次确认"))
            return;
        disconnect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        if(m_data->DeleteDatabase("recordDatabase.db","ID",m_IDList))
            m_parent->MessageHit("删除成功!");
        else
            m_parent->MessageHit("删除失败!");
        this->ButtonFindPress();
        connect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        return;
    }
}

void RecordWidget::ButtonDetailsPress(){//详情
    //获取选中
    int count=m_table->selectedItems().count()/m_col;
    if(count!=1)
        return;
    int index=m_table->selectedRanges().first().topRow();
    //内容
    m_itemWidget->reWidget(m_dataList->at(index));
    m_itemWidget->exec();
}

void RecordWidget::ButtonDetailsUnSale(){//详情-退货
    if(m_data->UnSaleBasedata(m_itemWidget->m_record)){
        if(!m_out){
            //获取选中
            int index=m_table->selectedRanges().first().topRow();
            //清本界面
            m_table->item(index,1)->setText("已退货");
            QString str1=m_result->text(),str2,str3;
            str2=str1.section("\t\t",0,0); str2.remove(0,5); str2.remove(str2.length()-1,1);
            str3=QString("搜索结果：%1条\t\t").arg(str2);
            str2=str1.section("\t\t",1,1); str2.remove(0,3); str2.remove(str2.length()-1,1);
            str3+=QString("退货：%1条\t\t").arg(str2.toInt()+1);
            str2=str1.section("\t\t",2,3); str2.remove(0,4); str2.remove(str2.length()-1,1);
            str3+=QString("总售价：%1元\t\t").arg(QString::number(str2.toFloat()-m_itemWidget->m_record->price,'d',2));
            str2=str1.section("\t\t",3,3); str2.remove(0,4); str2.remove(str2.length()-1,1);
            str3+=QString("总毛利：%3元").arg(QString::number(str2.toFloat()-m_itemWidget->m_record->gather,'d',2));
            m_result->setText(str3);

        }
        else
            this->clearTable();
        m_itemWidget->close();
        //清其他界面
        m_parent->recordUnSaleEnd();
        m_parent->MessageHit("退货成功！");
    }
    else
        m_parent->MessageHit("退货失败！");
}
void RecordWidget::ButtonDetailsPrint(){//详情-打印
    m_parent->m_printsum->m_printer->setDocName("再次打印:"+m_itemWidget->m_record->ID);
    connect(m_parent->m_printsum,&PrintSum::PrintBegin,this,&RecordWidget::ButtonDetailsPrint);
    m_parent->m_printsum->doPrint(m_itemWidget->m_record);
}
void RecordWidget::ButtonDetailsPrintFinished(){//详情-打印结束
    disconnect(m_parent->m_printsum,&PrintSum::PrintBegin,this,&RecordWidget::ButtonDetailsPrint);
    m_parent->MessageHit(QString("已加入到打印队列，单号:%1").arg(m_itemWidget->m_record->ID));
}
void RecordWidget::ButtonDetailsWatch(){//详情-预览
    m_parent->m_printsum->Watch(m_itemWidget->m_record);
}
void RecordWidget::ButtonDetailsItem(QString type,QString text){//详情-详情
    m_parent->ItemDetails(type,text);
}
void RecordWidget::ButtonDetailsItem(QString type,PhoneRecord* record){//详情-详情
    m_parent->ItemDetails(type,record);
}
/*----键盘响应----*/
void RecordWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return))
        m_btn1->click();
}
