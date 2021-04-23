//窗口
#include "assetswidget.h"
#include "assetsitemwidget.h"
#include "mainwindow.h"
#include "uloading.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
//控件
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include "ubutton.h"
#include "uline.h"
#include "urectbox.h"
//数据
#include "data.h"
//辅助
#include <QHeaderView>
#include <QKeyEvent>
#include <QCheckBox>
#include <QTableWidgetSelectionRange>

AssetsWidget::AssetsWidget(MainWindow *parent, Data *data)
    : QWidget(parent)
{
    //初始化
    m_parent=parent;
    m_data=data;
    m_dataList=NULL;
    createWidget();
    //子界面
    m_addWidget = new AssetsItemWidget(this,"ADD");
    m_editWidget = new AssetsItemWidget(this,"EDIT");
    m_detailsWidget = new AssetsItemWidget(this,"DETAILS");
}

AssetsWidget::~AssetsWidget(){
    clearTable();
    delete m_layout;
}

void AssetsWidget::createWidget(){//创建
    m_layout = new QVBoxLayout();
    //---限定---
    m_findLayout = new QHBoxLayout();
    //单成本价
    m_oneBox = new URectBox("单成本价","元","元");
    m_oneBox->m_checkBox->setShortcut(QKeySequence("F1"));
    //资产名称-备注
    m_findLabel = new QLabel("搜索:");
    m_edit = new QLineEdit();
    m_edit->setFocusPolicy(Qt::StrongFocus);
    m_edit->setPlaceholderText("资产名称/备注");
    m_edit->setMaxLength(50);
    m_edit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9\u4e00-\u9fa5]*$"), m_edit));
    //总成本价
    m_allBox = new URectBox("总成本价","元","元");
    m_allBox->m_checkBox->setShortcut(QKeySequence("F2"));
    //布局
    m_findLayout->addWidget(m_oneBox);
    m_findLayout->addWidget(m_allBox);
    m_findLayout->addWidget(m_findLabel);
    m_findLayout->addWidget(m_edit);
    //---结果---
    m_result = new QLabel();
    m_result->setFixedWidth(300);
    m_result->setAlignment(Qt::AlignCenter);
    m_findLayout->addWidget(m_result);
    //---按钮---
    m_btnLayout = new QHBoxLayout();
    //创建按钮
    m_btn1 = new UButton("查询(Enter)","",80,30,m_btnLayout);
    m_btn2 = new UButton("增加(Insert)","Insert",80,30,m_btnLayout);
    m_btn3 = new UButton("修改(Space)","Space",80,30,m_btnLayout);
    m_btn4 = new UButton("删除(Delete)","Delete",80,30,m_btnLayout);
    m_btn5 = new UButton("清空(Esc)","Esc",80,30,m_btnLayout);
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
    headerLabels<< "资产名称"<< "数量"<< "一个成本价"<< "总成本价"<<"备注";
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
    connect(m_table,&QTableWidget::doubleClicked,this,&AssetsWidget::ButtonDetailsPress);
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

void AssetsWidget::clearTable(){//重置
    int count;
    while((count=m_table->rowCount())>0)
        m_table->removeRow(count-1);
    for(int i=0;i<10;i++)
        m_table->insertRow(0);
    m_result->setText("");
    if(m_dataList!=NULL){
        foreach(AssetsData* data,(*m_dataList))
            delete data;
        delete m_dataList;
        m_dataList=NULL;
    }
}

void AssetsWidget::ButtonFindPressFinished(QList<AssetsData *> *dataList){//获取数据到表格-结束槽函数
    disconnect(m_data, &Data::SendAssetsData, this, &AssetsWidget::ButtonFindPressFinished);
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
    strList=&Type::Assets;
    //打印按钮
    m_btn5->setEnabled(true);
    //开始添加
    i=0;
    int count=0;
    double price=0;
    foreach(AssetsData *temp,(*dataList)){
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
        price+=temp->price*temp->count;
        i++;
    }
    //结果
    count=i;
    m_result->setText(QString("搜索结果：%1条\t\t\t\t总成本价：%2元").arg(count).arg(QString::number(price,'d',2)));
}

void AssetsWidget::ButtonFindPress(){//查询按钮
    clearTable();
    connect(m_data, &Data::SendAssetsData, this, &AssetsWidget::ButtonFindPressFinished,Qt::QueuedConnection);

    QString str1,str2;
    int count=0;
    str1=QString(" where(");
    //单成本
    if(m_oneBox->GetFrom()!=-1 && count>0)
        str1+=QString(" and ");
    if(m_oneBox->GetFrom()!=-1){
        str1+=QString("(%1>=%2 and %1<=%3)").arg("price").arg(m_oneBox->GetFrom()).arg(m_oneBox->GetTo());
        count++;
    }
    //总成本
    if(m_allBox->GetFrom()!=-1 && count>0)
        str1+=QString(" and ");
    if(m_allBox->GetFrom()!=-1){
        str1+=QString("(%1*%2>=%3 and %1*%2<=%4)").arg("price").arg("count").arg(m_allBox->GetFrom()).arg(m_allBox->GetTo());
        count++;
    }
    //搜索框
    str2 = m_edit->text();
    if(str2!="" && count>0)
        str1+=QString(" and ");
    if(str2.length()!=0){
        str1+=QString("%1 like '%%%3%%' or %2 like '%%%3%%'").arg("name").arg("addition").arg(str2);
        count++;
    }
    //结尾
    str1+=")";
    if(str1==" where()")
        str1="";
    m_parent->m_loading->Start("GetAssetsDatabase",str1);
}
//添加
void AssetsWidget::ButtonAddPress(){
    m_addWidget->reWidget();
    m_addWidget->exec();
}

void AssetsWidget::ButtonAddPressEnter (){
    //判断
    if(m_addWidget->m_name->GetValue().length()==0){
        m_parent->MessageHit("资产名不能为空");
        return;
    }
    if(m_addWidget->m_count->GetValue().toInt()==0){
        m_parent->MessageHit("数量不能为0");
        return;
    }
    //新建
    AssetsData *asset=new AssetsData();
    asset->name=m_addWidget->m_name->GetValue();
    asset->count=m_addWidget->m_count->GetValue().toInt();
    asset->price=m_addWidget->m_price->GetValue().toDouble();
    asset->addition=m_addWidget->m_addition->GetValue();
    //提示框
    QString str=QString("是否添加一项固定资产数据？\n名称：%1\n数量：%2\n单价：%3")
                        .arg(asset->name).arg(asset->count).arg(asset->price);
    if(asset->addition!="")
        str+=QString("\n备注：%1").arg(asset->addition);
    if(m_parent->MessageQuestion(str)){
        //添加
        if(m_data->InsertDatabase(asset)){
            m_addWidget->close();
            m_parent->MessageHit(QString("添加成功!"));
            ButtonFindPress();
        }
        else
            m_parent->MessageHit(QString("添加失败!"));
    }
    delete asset;
}

//修改
void AssetsWidget::ButtonEditPress(){
    //获取选中
    int count=m_table->selectedItems().count()/5;
    if(count==0)
        return;
    if(count>1){
        m_parent->MessageHit("不能批量修改");
        return;
    }
    int index=m_table->selectedRanges().first().topRow();
    //内容
    m_editWidget->reWidget(m_dataList->at(index));
    m_editWidget->exec();
}

void AssetsWidget::ButtonEditPressEnter (){
    //判断
    if(m_editWidget->m_name->GetValue().length()==0){
        m_parent->MessageHit("资产名不能为空");
        return;
    }
    if(m_editWidget->m_count->GetValue().toInt()==0){
        m_parent->MessageHit("数量不能为0");
        return;
    }
    //新建
    AssetsData *asset=new AssetsData();
    asset->name=m_editWidget->m_name->GetValue();
    asset->count=m_editWidget->m_count->GetValue().toInt();
    asset->price=m_editWidget->m_price->GetValue().toDouble();
    asset->addition=m_editWidget->m_addition->GetValue();
    //提示框
    QStringList strList;
    QString str;
    strList.clear();
    AssetsData *data=m_editWidget->m_record;
    if(data->name!=asset->name){
        strList.append("name");
        str=QString("\n名称: %1->%2").arg(data->name).arg(asset->name);
    }
    if(data->count!=asset->count){
        strList.append("count");
        str+=QString("\n数量: %1->%2").arg(data->count).arg(asset->count);
    }
    if(data->price!=asset->price){
        strList.append("price");
        str+=QString("\n单价: %1->%2").arg(data->price).arg(asset->price);
    }
    if(data->addition!=asset->addition){
        QString tempstr;
        strList.append("addition");
        tempstr=data->addition;
        if(tempstr=="")
            tempstr="空";
        str+=QString("\n备注      : %1").arg(tempstr);
        tempstr=asset->addition;
        if(tempstr=="")
            tempstr="空";
        str+=QString("->%1").arg(tempstr);
    }

    if(str.length()==0){
        delete asset;
        m_parent->MessageHit("未修改任何数据!");
        return;
    }

    str=QString("是否修改一项固定资产数据？")+str;
    //修改
    if(m_parent->MessageQuestion(str)){
        m_editWidget->close();
        //修改固定资产
        if(!m_data->Update(data->name,strList,asset)){
            m_parent->MessageHit("修改失败!");
            delete asset;
            return;
        }
        //清界面
        for(int i=m_table->rowCount()-1;i>=0;i--){
            if(m_table->item(i,0)!=NULL && m_table->item(i,0)->text()==data->name)//寻找
                foreach(QString str,strList){
                    if(str=="name")
                        m_table->item(i,0)->setText(asset->GetData("show","name"));
                    else if(str=="count"){
                        m_table->item(i,1)->setText(asset->GetData("show","count"));
                        m_table->item(i,3)->setText(asset->GetData("show","allPrice"));
                    }
                    else if(str=="price"){
                        m_table->item(i,2)->setText(asset->GetData("show","price"));
                        m_table->item(i,3)->setText(asset->GetData("show","allPrice"));
                    }
                    else if(str=="addition")
                        m_table->item(i,4)->setText(asset->GetData("show","addition"));
                }
        }
        m_editWidget->m_record->name=asset->name;
        m_editWidget->m_record->count=asset->count;
        m_editWidget->m_record->price=asset->price;
        m_editWidget->m_record->addition=asset->addition;
        //成功
        m_parent->MessageHit("修改成功!");
    }
    delete asset;
}

void AssetsWidget::ButtonDelPress(){//删除按钮
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
            m_IDList.append(m_table->item(i,0)->text());
        }
    //询问是否删除
    QString str;
    str=QString("是否删除%1项固定资产").arg(count);
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
        if(m_data->DeleteDatabase("countDatabase.db","Assets","name",m_IDList))
            m_parent->MessageHit("删除成功!");
        else
            m_parent->MessageHit("删除失败!");
        this->ButtonFindPress();
        connect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        return;
    }
}

void AssetsWidget::ButtonDelAllPress(){//清空
    m_table->selectAll();
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
            m_IDList.append(m_table->item(i,0)->text());
        }
    //询问是否删除
    QString str;
    str=QString("是否删除当前搜索的%1个数据？").arg(count);
    //删除
    if(m_parent->MessageQuestion(str)){
        if(!m_parent->MessageQuestion("请再次确认"))
            return;
        disconnect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        if(m_data->DeleteDatabase("countDatabase.db","Assets","name",m_IDList))
            m_parent->MessageHit("删除成功!");
        else
            m_parent->MessageHit("删除失败!");
        this->ButtonFindPress();
        connect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        return;
    }
}

void AssetsWidget::ButtonDetailsPress(){//详情
    //获取选中
    int count=m_table->selectedItems().count()/5;
    if(count!=1)
        return;
    int index=m_table->selectedRanges().first().topRow();
    //内容
    m_detailsWidget->reWidget(m_dataList->at(index));
    m_detailsWidget->exec();
}

/*----键盘响应----*/
void AssetsWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return))
        m_btn1->click();
}
