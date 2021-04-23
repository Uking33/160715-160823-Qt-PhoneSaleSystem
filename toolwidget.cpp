//窗口
#include "toolwidget.h"
#include "toolitemwidget.h"
#include "mainwindow.h"
#include "uloading.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
//数据
#include "data.h"
//控件
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include "uline.h"
#include "ubutton.h"
#include "udatebox.h"
#include "utimebox.h"
#include "urectbox.h"
//辅助
#include <QTableWidgetSelectionRange>
#include <QHeaderView>
#include <QKeyEvent>

ToolWidget::ToolWidget(MainWindow *parent, Data *data, bool model) : QWidget(parent){
    //初始化
    m_parent=parent;
    m_data=data;
    m_model=model;
    m_dataList=NULL;
    m_out=false;
    createWidget();
    //子界面
    if(m_model){
        m_addWidget = new ToolItemWidget(this,"ADD");
        m_editWidget = new ToolItemWidget(this,"EDIT");
    }
    m_detailsWidget = new ToolItemWidget(this,"DETAILS");
}

ToolWidget::~ToolWidget(){
    clearTable();
    delete m_layout;
}

void ToolWidget::createWidget(){//创建
    m_layout = new QVBoxLayout();
    //--查询范围--
    m_findLayout = new QHBoxLayout();
    //成本价
    m_buyingBox = new URectBox("成本价","元","元");
    m_buyingBox->SetShortcut("F1");
    //零售价
    m_saleBox = new URectBox("零售价","元","元");
    m_saleBox->SetShortcut("F2");
    //配件名称    
    m_findLabel = new QLabel("搜索:");
    m_findEdit = new QLineEdit();
    m_findEdit->setFocusPolicy(Qt::StrongFocus);
    m_findEdit->setMaxLength(30);
    m_findEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9\u4e00-\u9fa5]*$"), m_findEdit));
    m_findEdit->setPlaceholderText("配件名称");
    //结果
    m_result = new QLabel();
    m_result->setFixedWidth(200);
    m_result->setAlignment(Qt::AlignCenter);
    //布局
    m_findLayout->addWidget(m_buyingBox);
    m_findLayout->addWidget(m_saleBox);
    m_findLayout->addWidget(m_findLabel);
    m_findLayout->addWidget(m_findEdit);
    if(m_model){
        m_findLayout->setSpacing(10);
        m_findLayout->addWidget(m_result);
    }
    //--按钮--
    m_btnLayout = new QHBoxLayout();
    //创建按钮
    m_btn1 = new UButton("查询(Enter)","",80,30,m_btnLayout);
    m_btn2 = new UButton("出售(←)","Backspace",80,30,m_btnLayout);
    m_btn1->setEnabled(true);
    m_btn2->setEnabled(false);
    connect(m_btn1,SIGNAL(clicked()),this,SLOT(ButtonFindPress()));
    connect(m_btn2,SIGNAL(clicked()),this,SLOT(ButtonPrintPress()));
    if(m_model){
        m_btn3 = new UButton("添加(Insert)","Insert",80,30,m_btnLayout);
        m_btn4 = new UButton("修改(Space)","Space",80,30,m_btnLayout);
        m_btn5 = new UButton("删除(Delete)","Delete",80,30,m_btnLayout);
        m_btn3->setEnabled(true);
        m_btn4->setEnabled(false);
        m_btn5->setEnabled(false);
        connect(m_btn3,SIGNAL(clicked()),this,SLOT(ButtonAddPress()));
        connect(m_btn4,SIGNAL(clicked()),this,SLOT(ButtonEditPress()));
        connect(m_btn5,SIGNAL(clicked()),this,SLOT(ButtonDelPress()));
    }
    //--表格--
    QStringList headerLabels;
    if(m_model){
        m_col=5;
        headerLabels<< "名称"<< "数量"<< "成本价"<< "零售价"<< "单位";
    }
    else{
        m_col=4;
        headerLabels<< "名称"<< "数量"<< "零售价"<< "单位";
    }
    //创建表格
    m_table=new QTableWidget(10,m_col);
    m_table->setMaximumHeight(497);
    m_table->setMinimumHeight(497);
    m_table->setHorizontalHeaderLabels(headerLabels);//行标题
    m_table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_table->verticalHeader()->setDefaultSectionSize(47);//行高
    m_table->horizontalHeader()->setDefaultSectionSize(200);//列宽
    m_table->horizontalHeader()->setStretchLastSection(true);//右边界对齐
    m_table->verticalHeader()->setStretchLastSection(true);//下边界对齐
    m_table->setSelectionBehavior (QAbstractItemView::SelectRows);//设置表格的选择方式
    m_table->setEditTriggers(false);//设置编辑方式
    m_table->setFocusPolicy(Qt::ClickFocus);//设置焦点策略
    //设置连接
    connect(m_table,&QTableWidget::doubleClicked,this,&ToolWidget::ButtonDetailsPress);
    QObject::connect(m_table,&QTableWidget::itemSelectionChanged,[=](){
        m_btn2->setEnabled(true);
        if(m_model){
            m_btn4->setEnabled(true);
            m_btn5->setEnabled(true);
        }
    });
    //---布局---
    m_layout->addStretch(1);
    m_layout->addLayout(m_findLayout);
    if(m_model)
        m_layout->addLayout(m_btnLayout);
    else
        m_findLayout->addLayout(m_btnLayout);
    m_layout->addStretch(1);
    m_layout->addWidget(m_table);
    this->setLayout(m_layout);
    if(m_model)
        m_parent->setSize(1100,650);
    else
        m_parent->setSize(900,600);
}

void ToolWidget::clearTable(){//重置
    //表
    int count;
    while((count=m_table->rowCount())>0)
        m_table->removeRow(count-1);
    for(int i=0;i<10;i++)
        m_table->insertRow(0);
    //结果
    m_result->setText("");
    //m_dataList
    if(m_dataList!=NULL){
        foreach(ToolData* data,(*m_dataList))
            delete data;
        delete m_dataList;
        m_dataList=NULL;
    }
}

void ToolWidget::ItemDetailsFind(PhoneRecord *data){//外部强制查询
    //数据初始化
    ToolData *tool=new ToolData;
    tool->name=data->phoneModels;
    tool->count=data->count;
    tool->buyingPrice=data->buyingPrice;
    tool->salingPrice=data->salingPrice;
    tool->unit=data->unit;
    //设置
    m_detailsWidget->reWidget(tool);
    m_detailsWidget->m_count->m_label->setText("数量");
    m_detailsWidget->m_btn1->setEnabled(false);
    m_detailsWidget->m_btn2->setEnabled(false);
    m_detailsWidget->exec();
    delete tool;
}

void ToolWidget::ItemDetailsEdit(PhoneRecord *data){//修改外部配件
    ButtonFindPress();
    m_out=true;
    //数据初始化
    ToolData *tool=new ToolData;
    tool->name=data->phoneModels;
    tool->count=data->count;
    tool->buyingPrice=data->buyingPrice;
    tool->salingPrice=data->salingPrice;
    tool->unit=data->unit;
    //窗口初始化
    int index=0;
    int max=1;
    if(m_dataList!=NULL){
        foreach(ToolData *dd,(*m_dataList)){
            if(dd->name==data->phoneModels)
                break;
            index++;
        }
        max=m_dataList->at(index)->count+data->count;
    }
    m_editWidget->reWidget(tool);
    m_editWidget->m_name->SetReadOnly(true);
    m_editWidget->m_buyingPrice->SetReadOnly(true);
    m_editWidget->m_unit->SetReadOnly(true);
    m_editWidget->m_count->SetIntNotype(max);
    m_editWidget->exec();
    ButtonFindPress();
}

void ToolWidget::ItemShopFinished(QList<ToolData *>* dataList){//外部出售
    disconnect(m_data, &Data::SendToolData, this, &ToolWidget::ItemShopFinished);
    ToolData *t_data;
    t_data = dataList->first();
    int count=1;
    int max=t_data->count;
    if(!m_parent->shopingCount(count)){
        m_parent->MessageHit("该单超过4个项目,无法添加");
        return;
    }
    //输入数量
    p_dlg=new QDialog;
    p_dlg->setWindowTitle(QString("输入数量(1~%1)").arg(max));
    p_layout=new QVBoxLayout;
    p_btnLayout=new QHBoxLayout;
    p_btn1=new QPushButton("送");
    p_btn2=new QPushButton("出售");
    p_btn3=new QPushButton("取消");
    p_btn1->setEnabled(true);
    p_btn2->setEnabled(true);
    p_btnLayout->addWidget(p_btn1);
    p_btnLayout->addWidget(p_btn2);
    p_btnLayout->addWidget(p_btn3);
    p_count=new ULine("数量:",QString("1~%1").arg(max));
    p_count->SetInt(max,1);
    p_count->SetValue(1);
    connect(p_count,&ULine::textEdited,[=](){
        if(p_count->GetValue().toInt()==0){
            p_btn1->setEnabled(false);
            p_btn2->setEnabled(false);
        }
        else{
            p_btn1->setEnabled(true);
            p_btn2->setEnabled(true);
        }
    });
    p_layout->addLayout(p_count);
    p_layout->addLayout(p_btnLayout);
    p_dlg->setLayout(p_layout);
    connect(p_btn1,&QPushButton::clicked,[=](){
        m_printIsFree=true;
        p_dlg->accept();
    });
    connect(p_btn2,&QPushButton::clicked,[=](){
        m_printIsFree=false;
        p_dlg->accept();
    });
    connect(p_btn3,&QPushButton::clicked,p_dlg,&QDialog::close);
    if(!p_dlg->exec()){
        delete p_dlg;
        return;
    }
    m_printCount=p_count->GetValue().toInt();
    delete p_dlg;
    //添加
    t_data->count=m_printCount;
    if(m_printIsFree)
        t_data->salingPrice=0;
    m_parent->addShopingItem(t_data);
    //清配件
    for(int index=0;index<m_table->rowCount();index++){
        if(m_table->item(index,0)==NULL || m_table->item(index,0)->text()!=t_data->name)
            continue;
        m_dataList->at(index)->count=m_dataList->at(index)->count-m_printCount;
        m_table->item(index,1)->setText(QString::number(m_dataList->at(index)->count));
    }
    delete t_data;
    //提示
    m_parent->MessageHit(QString("已添加%1项配件到购物车").arg(count),800);
}
void ToolWidget::ItemShop(const QString &name){
    //寻找
    connect(m_data, &Data::SendToolData, this, &ToolWidget::ItemShopFinished,Qt::QueuedConnection);
    m_data->GetToolDatabase(QString(" WHERE name='%1'").arg(name));
}

void ToolWidget::ButtonFindPressFinished(QList<ToolData *> *dataList){//获取数据到表格-结束槽函数
    //断开连接
    disconnect(m_data, &Data::SendToolData, this, &ToolWidget::ButtonFindPressFinished);
    //初始化1
    m_btn1->setEnabled(true);
    m_btn2->setEnabled(false);
    if(m_model){
        m_btn3->setEnabled(true);
        m_btn4->setEnabled(false);
        m_btn5->setEnabled(false);
    }
    //是否为空
    if(dataList==NULL){
        m_result->setText("");
        delete dataList;
        return;
    }
    m_dataList=dataList;
    //初始化2
    int i,j;
    int count=0;
    QStringList *strList;
    if(m_model) strList=&Type::TOOL_ALL;
    else strList=&Type::TOOL_PART;
    //添加
    i=0;
    foreach(ToolData *temp,(*dataList)){
        j=0;
        if(m_table->rowCount()<=i){
            m_table->insertRow(i);
            m_table->setRowHeight(i,47);
        }
        foreach(QString str,(*strList)){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            if(str=="count"){
                temp->count-=m_parent->shopingToolCount(temp->name);
                item->setText(temp->GetData("show",str));
            }
            else
                item->setText(temp->GetData("show",str));
            m_table->setItem(i,j,item);
            j++;
        }
        i++;
    }
    //结果
    count=i;
    m_result->setText(QString("搜索结果：%1种配件").arg(count));
}
void ToolWidget::ButtonFindPress(){//查询按钮
    //清除表并连接
    clearTable();
    connect(m_data, &Data::SendToolData, this, &ToolWidget::ButtonFindPressFinished,Qt::QueuedConnection);
    //添加查找范围
    QString str1,str2;
    int count=0;
    str1=QString(" where(");
    //搜索框-名字
    str2 = m_findEdit->text();
    str2=str2.simplified();
    if(str2=="")
        m_findEdit->setText("");
    if(str2!="" && count>0)
        str1+=QString(" and ");
    if(str2!=""){
        str1+=QString("%1 like '%%%2%%'").arg("name").arg(str2);
        count++;
    }
    //进货价
    if(m_buyingBox->GetFrom()!=-1 && count>0)
        str1+=QString(" and ");
    if(m_buyingBox->GetFrom()!=-1){
        str1+=QString("(%1>=%2 and %1<=%3)").arg("buyingPrice").arg(m_buyingBox->GetFrom()).arg(m_buyingBox->GetTo());
        count++;
    }
    //零售价
    if(m_saleBox->GetFrom()!=-1 && count>0)
        str1+=QString(" and ");
    if(m_saleBox->GetFrom()!=-1){
        str1+=QString("(%1>=%2 and %1<=%3)").arg("salingPrice").arg(m_saleBox->GetFrom()).arg(m_saleBox->GetTo());
        count++;
    }
    //结尾
    str1+=")";
    if(str1==" where()")
        str1="";
    m_parent->m_loading->Start("GetToolDatabase",str1);
}

void ToolWidget::ButtonAddPress(){//添加
    m_addWidget->reWidget();
    m_addWidget->exec();
}
void ToolWidget::ButtonAddPressEnter (){//添加新配件
    //新建
    ToolData *tool=new ToolData();
    tool->name=m_addWidget->m_name->GetValue();
    tool->count=m_addWidget->m_count->GetValue().toInt();
    tool->buyingPrice=m_addWidget->m_buyingPrice->GetValue().toFloat();
    tool->salingPrice=m_addWidget->m_salingPrice->GetValue().toFloat();
    tool->unit=m_addWidget->m_unit->GetValue();
    //提示框
    QString str=QString("是否添加一个配件数据？\n名称：%1\n数量：%2\n进货价：%3\n零售价：%4\n单位：%5")
                        .arg(tool->name).arg(tool->count).arg(tool->buyingPrice).arg(tool->salingPrice).arg(tool->unit);
    if(m_parent->MessageQuestion(str)){
        //添加
        if(m_data->InsertDatabase(tool)){
            m_addWidget->close();
            m_parent->MessageHit(QString("添加成功!"));
            ButtonFindPress();
        }
        else
            m_parent->MessageHit(QString("添加失败!"));
    }
    delete tool;
}

void ToolWidget::ButtonDelPress(){//删除按钮
    //获取选中
    int count=m_table->selectedItems().count()/m_col;
    if(count==0)
        return;
    m_IDList.clear();
    m_selectionRange=m_table->selectedRanges();
    //记录name
    m_IDList.clear();
    foreach(QTableWidgetSelectionRange range,m_selectionRange)
        for(int i=range.topRow();i<=range.bottomRow();i++){
            if(m_table->item(i,0)==NULL)
                break;
            m_IDList.append(m_table->item(i,0)->text());
        }
    //询问是否删除
    QString str;
    str=QString("是否删除%1个数据").arg(count);
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

        if(m_data->DeleteDatabase("countDatabase.db","Tool","name",m_IDList))
            m_parent->MessageHit("删除成功!");
        else
            m_parent->MessageHit("删除失败!");
        this->ButtonFindPress();
        connect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        return;
    }
}

void ToolWidget::ButtonEditPress(){//修改按钮
    //获取选中
    int count=m_table->selectedItems().count()/m_col;
    if(count==0)
        return;
    m_selectionRange=m_table->selectedRanges();
    //记录
    int index=0;
    count=0;
    foreach(QTableWidgetSelectionRange range,m_selectionRange)
        for(int i=range.topRow();i<=range.bottomRow();i++){
            if(m_table->item(i,0)==NULL)
                break;
            index=i;
            count++;
        }
    if(count>1){
        m_parent->MessageHit("配件只能逐个修改!");
        return;
    }
    //开启
    m_editWidget->reWidget(m_dataList->at(index));
    m_editWidget->exec();
}

void ToolWidget::ButtonEditPressEnter(){
    //提示框
    ToolData *data=m_editWidget->m_tool;
    ToolData *t_data=new ToolData;
    QStringList strList;
    strList.clear();
    QString str;
    if(data->name!=m_editWidget->m_name->GetValue()){
        strList.append("name");
        t_data->name=m_editWidget->m_name->GetValue();
        str=QString("\n名称: %1->%2").arg(data->name).arg(m_editWidget->m_name->GetValue());
    }
    if(data->count!=m_editWidget->m_count->GetValue().toInt()){
        strList.append("count");
        t_data->count=m_editWidget->m_count->GetValue().toInt();
        str+=QString("\n数量: %1->%2").arg(data->count).arg(m_editWidget->m_count->GetValue());
    }
    if(m_model && data->buyingPrice!=m_editWidget->m_buyingPrice->GetValue().toFloat()){
        strList.append("buyingPrice");
        t_data->buyingPrice=m_editWidget->m_buyingPrice->GetValue().toFloat();
        str+=QString("\n进货价(¥): %1->%2").arg(data->buyingPrice).arg(m_editWidget->m_buyingPrice->GetValue());
    }
    if(data->salingPrice!=m_editWidget->m_salingPrice->GetValue().toFloat()){
        strList.append("salingPrice");
        t_data->salingPrice=m_editWidget->m_salingPrice->GetValue().toFloat();
        str+=QString("\n零售价(¥): %1->%2").arg(data->salingPrice).arg(m_editWidget->m_salingPrice->GetValue());
    }
    if(data->unit!=m_editWidget->m_unit->GetValue()){
        strList.append("unit");
        t_data->unit=m_editWidget->m_unit->GetValue();
        str+=QString("\n单位: %1->%2").arg(data->unit).arg(m_editWidget->m_unit->GetValue());
    }

    if(str.length()==0){
        m_parent->MessageHit("未修改任何数据!");
        return;
    }
    str=QString("是否修改%1?").arg(data->name)+str;

    //判断是否为外部查询
    if(m_out){
        if(m_parent->MessageQuestion(str)){
            m_editWidget->close();
            PhoneRecord *record=new PhoneRecord;
            record->IMEI="";
            record->phoneModels=m_editWidget->m_name->GetValue();
            record->count=m_editWidget->m_count->GetValue().toInt();
            record->buyingPrice=m_editWidget->m_buyingPrice->GetValue().toFloat();
            record->salingPrice=m_editWidget->m_salingPrice->GetValue().toFloat();
            record->unit=m_editWidget->m_unit->GetValue();
            //修改tool数据
            m_IDList.clear();
            m_IDList.append(record->phoneModels);
            UnSale(m_IDList);
            m_printCount=record->count;
            for(int index=0;index<m_dataList->count();index++){
                if(m_table->item(index,0)==NULL || !m_IDList.contains(m_dataList->at(index)->name))
                    break;
                m_dataList->at(index)->count=m_dataList->at(index)->count-m_printCount;
                m_table->item(index,1)->setText(QString::number(m_dataList->at(index)->count));
            }
            //修改shop数据
            m_parent->ItemDetailsEditFinished("tool",record);
            //成功
            m_parent->MessageHit("已修改购物车中的数据!");
            delete record;
        }
        //退出
        m_out=false;
        delete t_data;
        return;
    }
    //添加
    if(m_parent->MessageQuestion(str)){
        m_editWidget->close();
        if(!m_data->Update(data->name,strList,t_data)){
            m_parent->MessageHit("修改失败!");
            return;
        }
        ButtonFindPress();
        //成功
        m_parent->MessageHit("修改成功!");
    }
    delete t_data;
}

void ToolWidget::ButtonDetailsPress(){//详情
    //获取选中
    int count=m_table->selectedItems().count()/m_col;
    if(count!=1)
        return;
    int index=m_table->selectedRanges().first().topRow();
    //内容
    m_detailsWidget->reWidget(m_dataList->at(index));
    if(m_dataList->at(index)->count==0 ||
            m_parent->shopingFind(m_table->item(index,0)->text()) ){//已售空或已添加
        m_detailsWidget->m_btn1->setEnabled(false);
        m_detailsWidget->m_btn2->setEnabled(false);
    }
    m_detailsWidget->exec();
}

void ToolWidget::ButtonPrintPress(){//出售
    int count=0;
    int max=0;
    //获取选中
    m_selectionRange=m_table->selectedRanges();
    if(m_table->selectedItems().count()/m_col==0)
        return;
    foreach(QTableWidgetSelectionRange range,m_selectionRange){
        for(int index=range.topRow();index<=range.bottomRow();index++){
            if(m_table->item(index,0)==NULL)
                break;
            if(m_dataList->at(index)->count==0 ||
                    m_parent->shopingFind(m_table->item(index,0)->text()) )//已售空或已添加
                continue;
            if(max==0 || max>m_table->item(index,1)->text().toInt())
                max=m_table->item(index,1)->text().toInt();
            count++;
        }
    }
    //判断是否超过
    if(count==0){
        m_parent->MessageHit(QString("选中的配件已添加或无库存"));
        return;
    }
    if(!m_parent->shopingCount(count)){
        m_parent->MessageHit("该单超过4个项目,无法添加");
        return;
    }
    //输入数量
    p_dlg=new QDialog;
    p_layout=new QVBoxLayout;
    p_btnLayout=new QHBoxLayout;
    p_btn1=new QPushButton("送");
    p_btn2=new QPushButton("出售");
    p_btn3=new QPushButton("取消");
    p_dlg->setWindowTitle(QString("输入数量(1~%1)").arg(max));
    p_btn1->setEnabled(true);
    p_btn2->setEnabled(true);
    p_btnLayout->addWidget(p_btn1);
    p_btnLayout->addWidget(p_btn2);
    p_btnLayout->addWidget(p_btn3);
    p_count=new ULine("数量:",QString("1~%1").arg(max));
    p_count->SetInt(max,1);
    p_count->SetValue(1);
    connect(p_count,&ULine::textEdited,[=](){
        if(p_count->GetValue().toInt()==0){
            p_btn1->setEnabled(false);
            p_btn2->setEnabled(false);
        }
        else{
            p_btn1->setEnabled(true);
            p_btn2->setEnabled(true);
        }
    });
    p_layout->addLayout(p_count);
    p_layout->addLayout(p_btnLayout);
    p_btn1->setFocus();
    p_dlg->setLayout(p_layout);
    connect(p_btn1,&QPushButton::clicked,[=](){
        m_printIsFree=true;
        p_dlg->accept();
    });
    connect(p_btn2,&QPushButton::clicked,[=](){
        m_printIsFree=false;
        p_dlg->accept();
    });
    connect(p_btn3,&QPushButton::clicked,p_dlg,&QDialog::close);
    if(!p_dlg->exec()){
        m_printCount=-1;
        delete p_dlg;
        return;
    }
    m_printCount=p_count->GetValue().toInt();
    delete p_dlg;
    //添加
    count=0;
    foreach(QTableWidgetSelectionRange range,m_selectionRange){
        for(int index=range.topRow();index<=range.bottomRow();index++){
            if(m_table->item(index,0)==NULL)
                break;
            if(m_dataList->at(index)->count!=0 &&
                    m_parent->shopingFind(m_table->item(index,0)->text()) )//已售空或已添加
                continue;
            ToolData *t_data=new ToolData();
            (*t_data)=(*m_dataList->at(index));
            t_data->count=m_printCount;
            if(m_printIsFree)
                t_data->salingPrice=0;
            if(m_parent->addShopingItem(t_data))
                count++;
            delete t_data;
        }
    }
    //清配件
    foreach(QTableWidgetSelectionRange range,m_selectionRange){
        for(int index=range.topRow();index<=range.bottomRow();index++){
            if(m_table->item(index,0)==NULL)
                break;
            m_dataList->at(index)->count=m_dataList->at(index)->count-m_printCount;
            m_table->item(index,1)->setText(QString::number(m_dataList->at(index)->count));
        }
    }
    //提示
    m_parent->MessageHit(QString("已添加%1项配件到购物车").arg(count),800);
}

void ToolWidget::ButtonDetailsSaleAddPress(){
    m_detailsWidget->close();
    ButtonPrintPress();
}

void ToolWidget::ButtonDetailsSaleSumPress(){
    m_detailsWidget->close();
    m_parent->addShopingItemAndSum();
}

void ToolWidget::UnSale(QStringList list){
    for(int index=0;index<m_table->rowCount();index++){
        if(m_table->item(index,0)==NULL || !list.contains(m_table->item(index,0)->text()))
            continue;
        m_dataList->at(index)->count=m_dataList->at(index)->count+m_parent->shopingToolCount(m_table->item(index,0)->text());
        m_table->item(index,1)->setText(QString::number(m_dataList->at(index)->count));
    }
}

/*----键盘响应----*/
void ToolWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return)){
        m_btn1->click();
    }
}
