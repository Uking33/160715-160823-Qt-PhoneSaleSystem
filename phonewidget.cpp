//窗口
#include "phonewidget.h"
#include "phoneitemwidget.h"
#include "mainwindow.h"
#include "printwidget.h"
#include "phoneloading.h"
#include "uloading.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
//控件
#include "ubutton.h"
#include "uline.h"
#include "udatebox.h"
#include "utimebox.h"
#include "urectbox.h"
#include <QTableWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
//数据
#include "data.h"
//辅助
#include <QHeaderView>

PhoneWidget::PhoneWidget(MainWindow *parent, Data *data, bool edit){
    //初始化
    m_parent=parent;
    m_data=data;
    m_model=edit;
    m_out=false;
    if(edit)
        m_col=10;
    else
        m_col=6;
    createWidget();
    createDetailsWidget();
    if(m_model){
        createAddWidget();
        createEditWidget();
    }
    m_dataList=NULL;
    m_loading = new PhoneLoading(m_findTable);
}
PhoneWidget::~PhoneWidget(){
    /*----界面Qt自动删除----*/
    clearTable();
    delete m_loading;
    delete m_layout;
}

/*----查询界面----*/
void PhoneWidget::createWidget(){//创建
    m_layout = new QVBoxLayout();
    //---通用控件---
    if(m_model){
        m_findIndateSLabel = new QLabel("入库起始");
        m_findIndateSBox = new UDateBox(2);
        m_findIndateSLayout= new QHBoxLayout;
        m_findIndateSLayout->addWidget(m_findIndateSLabel);
        m_findIndateSLayout->addWidget(m_findIndateSBox);
        m_findIndateSBtn1 = new UButton("日","F1",25,25,m_findIndateSLayout);
        m_findIndateSBtn1->setFocusPolicy(Qt::NoFocus);
        connect(m_findIndateSBtn1,&UButton::clicked,m_findIndateSBox,&UDateBox::Today);
        m_findIndateSBtn2 = new UButton("周","F2",25,25,m_findIndateSLayout);
        m_findIndateSBtn2->setFocusPolicy(Qt::NoFocus);
        connect(m_findIndateSBtn2,&UButton::clicked,m_findIndateSBox,&UDateBox::ToWeek);
    }
    else{
        m_findIndateSLabel = new QLabel("入库时间");
        m_findIndateSBox = new UDateBox(4);
        m_findIndateSLayout= new QHBoxLayout;
        m_findIndateSLayout->addWidget(m_findIndateSLabel);
        m_findIndateSLayout->addWidget(m_findIndateSBox);
        m_findIndateSBtn1 = new UButton("今","F1",25,25,m_findIndateSLayout);
        m_findIndateSBtn1->setFocusPolicy(Qt::NoFocus);
        connect(m_findIndateSBtn1,&UButton::clicked,m_findIndateSBox,&UDateBox::Today);
        m_findIndateSBtn2 = new UButton("全","F2",25,25,m_findIndateSLayout);
        m_findIndateSBtn2->setFocusPolicy(Qt::NoFocus);
        connect(m_findIndateSBtn2,&UButton::clicked,m_findIndateSBox,&UDateBox::First);
    }

    m_findSaleBox = new URectBox("销售价","元","元");
    m_findLabel = new QLabel("搜索:");
    m_findEdit = new QLineEdit();
    m_findEdit->setFocusPolicy(Qt::StrongFocus);
    m_findSaleBox->m_checkBox->setShortcut(QKeySequence("F3"));
    m_findEdit->setMaxLength(50);
    m_findEdit->setValidator(new QRegExpValidator(QRegExp("[^, ☺]*$"), m_findEdit));
    if(m_model)
        m_findEdit->setPlaceholderText("IMEI/型号/品牌/销售员/营业员/备注");
    else
        m_findEdit->setPlaceholderText("IMEI/型号/品牌/备注");

    m_findConLayout = new QHBoxLayout();
    m_findConLayout->setAlignment(Qt::AlignLeft);
    m_findConLayout->addLayout(m_findIndateSLayout);
    m_findConLayout->addWidget(m_findSaleBox);
    if(m_model){
        //未出售
        m_saleLayout =new QHBoxLayout();
        m_saleLayout->setSpacing(0);
        m_saleLayout->setContentsMargins(8,5,8,5);
        m_saleLabel = new QLabel("未出售");
        m_saleCheck = new QCheckBox();
        m_saleCheck->setShortcut(QKeySequence("F4"));
        m_saleCheck->setFocusPolicy(Qt::NoFocus);
        m_saleCheck->setChecked(true);
        m_saleLayout->addWidget(m_saleLabel);
        m_saleLayout->addWidget(m_saleCheck);
        m_findConLayout->addLayout(m_saleLayout);
    }
    m_findConLayout->addWidget(m_findLabel);
    m_findConLayout->addWidget(m_findEdit);

    //---管理员控件---
    if(m_model){
        //结果
        m_result=new QLabel();
        m_result->setFixedWidth(200);
        m_result->setAlignment(Qt::AlignCenter);
        m_findConLayout->addWidget(m_result);

        m_findInDateELabel = new QLabel("入库结束");
        m_findInDateEBox = new UDateBox(3,m_findIndateSBox);
        m_findIndateSBox->ChooseDay(m_findIndateSBox->m_comboBoxDay->currentText());
        m_findInDateELayout= new QHBoxLayout;
        m_findInDateELayout->addWidget(m_findInDateELabel);
        m_findInDateELayout->addWidget(m_findInDateEBox);
        m_findInDateEBtn1 = new UButton("月","F5",25,25,m_findInDateELayout);
        m_findInDateEBtn1->setFocusPolicy(Qt::NoFocus);
        connect(m_findInDateEBtn1,&UButton::clicked,m_findIndateSBox,&UDateBox::ToMonth);
        m_findInDateEBtn2 = new UButton("年","F6",25,25,m_findInDateELayout);
        m_findInDateEBtn2->setFocusPolicy(Qt::NoFocus);
        connect(m_findInDateEBtn2,&UButton::clicked,m_findIndateSBox,&UDateBox::ToYear);

        m_findBuyBox = new URectBox("进货价","元","元");
        m_findBuyBox->m_checkBox->setShortcut(QKeySequence("F7"));

        m_findAdLayout = new QHBoxLayout();
        m_findAdLayout->setAlignment(Qt::AlignLeft);
        m_findAdLayout->addLayout(m_findInDateELayout);
        m_findAdLayout->addWidget(m_findBuyBox);
    }
    //---按钮---
    m_findBtnLayout = new QHBoxLayout();
    //创建按钮
    m_findBtn1 = new UButton("查询(Enter)","",80,30,m_findBtnLayout);
    m_findBtn2 = new UButton("出售(←)","Backspace",80,30,m_findBtnLayout);
    if(m_model){
        m_findBtn3 = new UButton("添加(Insert)","Insert",80,30,m_findBtnLayout);
        m_findBtn4 = new UButton("修改(Space)","Space",80,30,m_findBtnLayout);
        m_findBtn5 = new UButton("删除(Delete)","Delete",80,30,m_findBtnLayout);
        m_findBtn6 = new UButton("打印(Ctrl+P)","Ctrl+P",80,30,m_findBtnLayout);
    }
    //信号连接
    connect(m_findBtn1,SIGNAL(clicked()),this,SLOT(ButtonFindPress()));
    connect(m_findBtn2,SIGNAL(clicked()),this,SLOT(ButtonShopPress()));
    m_findBtn1->setFocusPolicy(Qt::NoFocus);
    m_findBtn2->setFocusPolicy(Qt::NoFocus);
    m_findBtn2->setEnabled(false);
    if(m_model){
        connect(m_findBtn3,SIGNAL(clicked()),this,SLOT(ButtonAddPress()));
        connect(m_findBtn4,SIGNAL(clicked()),this,SLOT(ButtonEditPress()));
        connect(m_findBtn5,SIGNAL(clicked()),this,SLOT(ButtonDelPress()));
        connect(m_findBtn6,SIGNAL(clicked()),this,SLOT(ButtonPrintPress()));
        m_findBtn4->setEnabled(false);
        m_findBtn5->setEnabled(false);
        m_findBtn6->setEnabled(false);
    }
    //---添加表格---
    QStringList headerLabels;
    int rows,columns;
    rows=10;
    if(m_model){
        columns=10;
        headerLabels<< "串码"<< "手机品牌"<< "手机型号"<< "进货价"<< "销售价"
            << "入库时间"<< "销售时间"<< "营业员"<< "收银员"<<"备注";
    }
    else{
        columns=6;
        headerLabels<< "串码"<< "手机品牌"<< "手机型号"<< "销售价"
            << "入库时间"<< "备注";
    }
    //创建表格
    m_findTable=new QTableWidget(rows,columns);
    m_findTable->setMaximumHeight(497);
    m_findTable->setMinimumHeight(497);
    //行标题
    m_findTable->setHorizontalHeaderLabels(headerLabels);
    m_findTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_findTable->verticalHeader()->setDefaultSectionSize(47);//行高
    m_findTable->horizontalHeader()->setDefaultSectionSize(85);//列宽
    m_findTable->setColumnWidth(0,120);
    if(m_model){
       m_findTable->setColumnWidth(5,100);
       m_findTable->setColumnWidth(6,100);
    }
    else
        m_findTable->setColumnWidth(5,100);
    //设置对齐
    m_findTable->horizontalHeader()->setStretchLastSection(true);//右边界对齐
    m_findTable->verticalHeader()->setStretchLastSection(true);//下边界对齐
    //设置方式
    m_findTable->setSelectionBehavior ( QAbstractItemView::SelectRows);//设置表格的选择方式
    m_findTable->setEditTriggers(false);//设置编辑方式
    m_findTable->setFocusPolicy(Qt::ClickFocus);
    connect(m_findTable,&QTableWidget::doubleClicked,this,&PhoneWidget::ButtonDetailsPress);
    QObject::connect(m_findTable,&QTableWidget::itemSelectionChanged,[=](){
        m_findBtn2->setEnabled(true);
        if(m_model){
            m_findBtn4->setEnabled(true);
            m_findBtn5->setEnabled(true);
        }
    });
    //---布局---
    if(m_model){
        m_findBtnLayout->setSpacing(5);
        m_layout->addStretch(1);
        m_layout->addLayout(m_findConLayout);
        m_layout->addLayout(m_findAdLayout);
        m_layout->addStretch(1);
        m_findAdLayout->addLayout(m_findBtnLayout);
        m_layout->addWidget(m_findTable);
    }
    else{
        m_layout->addLayout(m_findConLayout);
        m_findConLayout->addLayout(m_findBtnLayout);
        m_layout->addWidget(m_findTable);
    }
    this->setLayout(m_layout);
    if(m_model)
        m_parent->setSize(1100,650);
    else
        m_parent->setSize(900,600);
}

void PhoneWidget::clearTable(){//重置
    int count;
    while((count=m_findTable->rowCount())>0)
        m_findTable->removeRow(count-1);
    for(int i=0;i<10;i++)
        m_findTable->insertRow(0);
    m_findBtn2->setEnabled(false);
    if(m_model){
        m_findBtn4->setEnabled(false);
        m_findBtn5->setEnabled(false);
        m_result->setText("");
    }
    //m_dataList
    if(m_dataList!=NULL){
        foreach(PhoneData* data,(*m_dataList))
            delete data;
        delete m_dataList;
        m_dataList=NULL;
    }
}

void PhoneWidget::ItemDetailsEditFinished(QList<PhoneData *>* dataList){//外部编辑查找结束
    disconnect(m_data, &Data::SendPhoneData, this, &PhoneWidget::ItemDetailsEditFinished);
    if(dataList==NULL){
        return;
    }
    //内容
    t_data = dataList->first();
    dataList->clear();
    delete dataList;
    //串码IMEI
    m_IDList.clear();
    m_IDList.append(t_data->IMEI);
    m_editWidget->reWidget(t_data,true);
    m_editWidget->m_count=1;
    m_editWidget->exec();
    delete t_data;
    m_out=false;
}
void PhoneWidget::ItemDetailsEdit(PhoneRecord *data){//外部编辑
    m_out=true;
    //寻找
    connect(m_data, &Data::SendPhoneData, this, &PhoneWidget::ItemDetailsEditFinished);
    m_data->GetDatabase("phoneDatabase.db","ALL",QString(" WHERE IMEI='%1'").arg(data->IMEI));
}

void PhoneWidget::ItemDetailsFindFinished(QList<PhoneData *>* dataList){//外部强制查询-结束槽函数
    disconnect(m_data, &Data::SendPhoneData, this, &PhoneWidget::ItemDetailsFindFinished);
    if(dataList==NULL){
        return;
    }
    //内容
    t_data = dataList->first();
    dataList->clear();
    delete dataList;
    //串码IMEI
    if(t_data->outTime!="" || m_parent->shopingFind(t_data->IMEI)) t_data->outTime=t_data->GetData("phone","outTime");
    m_detailsWidget->reWidget(t_data);
    m_detailsWidget->exec();
    delete t_data;

}
void PhoneWidget::ItemDetailsFind(const QString ID){//外部强制查询
    //寻找
    connect(m_data, &Data::SendPhoneData, this, &PhoneWidget::ItemDetailsFindFinished);
    m_data->GetDatabase("phoneDatabase.db","ALL",QString(" WHERE IMEI='%1'").arg(ID));
}

void PhoneWidget::ButtonFindPressFinished(QList<PhoneData *>* dataList){//获取数据到表格-结束槽函数
    //断开连接
    disconnect(m_data, &Data::SendPhoneData, this, &PhoneWidget::ButtonFindPressFinished);
    //初始化
    if(dataList==NULL){
        if(m_model) m_findBtn6->setEnabled(false);
        return;
    }
    if(m_model) m_findBtn6->setEnabled(true);
    m_dataList=dataList;
    int i,j;
    int count;
    long long gather=0;
    QStringList *strList;
    if(m_model)
        strList=&Type::PHONE_ALL;
    else
        strList=&Type::PHONE_PART;
    i=0;
    //员工查询已出售
    if(!m_model && m_dataList!=NULL && m_dataList->count()!=0 && m_findEdit->text().length()==15 && m_findEdit->text().toLongLong()!=0
            && m_dataList->first()->outTime!=""){
        m_parent->MessageHit(m_dataList->first()->IMEI + "已出售");
        foreach(PhoneData* data,(*m_dataList))
            delete data;
        delete m_dataList;
        m_dataList=NULL;
        return;
    }
    //开始添加
    foreach(PhoneData *temp,(*m_dataList)){
        j=0;
        if(m_findTable->rowCount()<=i){
            m_findTable->insertRow(i);
            m_findTable->setRowHeight(i,47);
        }
        foreach(QString str,(*strList)){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            if(m_model && str=="outTime" && m_parent->shopingFind(temp->IMEI))//已添加
                item->setText("已添加");
            else
                item->setText(temp->GetData("phone",str));
            m_findTable->setItem(i,j,item);
            j++;
        }
        if(m_model && ""!=temp->outTime){
            gather+=temp->salingPrice-temp->buyingPrice;
        }
        i++;
    }
    //结果
    count=i;
    if(m_model){
        m_result->setText(QString("搜索结果：%1条 总毛利：%2元").arg(count).arg(gather));
    }
    if(count==1 && m_findEdit->text().length()==15 && m_findEdit->text().toLongLong()!=0){
        if(m_dataList->first()->outTime!=""){//已出售
            if((m_model && m_saleCheck->isChecked()))
                m_parent->MessageHit(m_dataList->first()->IMEI + "已出售");
            else if(m_model && !m_saleCheck->isChecked()){
                m_findTable->selectRow(0);
                ButtonDetailsPress();
            }
        }
        else{
            m_findTable->selectRow(0);
            ButtonDetailsPress();
        }
    }
}
void PhoneWidget::ButtonFindPress(){//查询按钮
    clearTable();
    connect(m_data, &Data::SendPhoneData, this, &PhoneWidget::ButtonFindPressFinished,Qt::QueuedConnection);

    QString str1,str2;
    int count=0;
    //入库时间
    if(m_model){
        str1=QString(" WHERE %1>='%2' and %1<='%3'").arg("inTime").arg(m_findIndateSBox->GetData()+"000000").arg(m_findInDateEBox->GetData()+"235959");
        count++;
    }
    else{
        if(m_findIndateSBox->GetData()=="ALL")
            str1=QString(" WHERE ");
        else{
            str1=QString(" WHERE %1 like '%2%%'").arg("inTime").arg(m_findIndateSBox->GetData());
            count++;
        }
    }
    //未出售或者搜索串码
    if((!m_model||(m_model && m_saleCheck->isChecked())) && !(m_findEdit->text().length()==15 && m_findEdit->text().toLongLong()!=0) ){
        if(count>0)
            str1+=QString(" and ");
        str1+=QString("%1=''").arg("outTime");
        count++;
    }
    //销售价
    if(m_findSaleBox->GetFrom()!=-1 && count>0)
        str1+=QString(" and ");
    if(m_findSaleBox->GetFrom()!=-1){
        str1+=QString("(%1>=%2 and %1<=%3)").arg("salingPrice").arg(m_findSaleBox->GetFrom()).arg(m_findSaleBox->GetTo());
        count++;
    }
    //搜索框
    str2 = m_findEdit->text();
    str2=str2.simplified();
    if(str2=="")
        m_findEdit->setText("");
    if(str2!="" && count>0)
        str1+=QString(" and ");
    if(str2.length()==15 && str2.toLongLong()!=0){
        str1+=QString("%1 like '%%%2%%'").arg("IMEI").arg(str2);
        count++;
    }
    else if(str2.length()!=15 && str2!=""){
        if(str2.length()<15 && str2.toLongLong()!=0)
            str1+=QString("(%1 like '%%%2%%' or").arg("IMEI").arg(str2);
        else
            str1+=QString("(");
        if(m_model)//销售/营业员
            str1+=QString("(phoneBrand like '%%%1%%' or phoneModels like '%%%1%%' or addition like '%%%1%%') or (shopAssistant like '%%%1%%' or cashier like '%%%1%%'))").arg(str2);
        else
            str1+=QString("(phoneBrand like '%%%1%%' or phoneModels like '%%%1%%' or addition like '%%%1%%'))").arg(str2);
        count++;
    }
    if(m_model){
        //进货价
        if(m_findBuyBox->GetFrom()!=-1 && count>0)
            str1+=QString(" and ");
        if(m_findBuyBox->GetFrom()!=-1){
            str1+=QString("(%1>=%2 and %1<=%3)").arg("buyingPrice").arg(m_findBuyBox->GetFrom()).arg(m_findBuyBox->GetTo());
            count++;
        }
    }
    //结尾
    if(str1==" WHERE ")
        str1="";
    if(m_model){
        if(m_findIndateSBox->GetData().left(4)==m_findInDateEBox->GetData().left(4))
            str2=m_findIndateSBox->GetTable();
        else
            str2="ALL";
    }
    else{
        if(m_findIndateSBox->GetData()!="ALL")
            str2=m_findIndateSBox->GetTable();
        else
            str2="ALL";
    }
    m_parent->m_loading->Start("GetDatabase","phoneDatabase.db",str2,str1);
}

void PhoneWidget::ButtonShopPress(){//添加到购物车
    int count=0;
    m_selectionRange=m_findTable->selectedRanges();
    if(m_findTable->selectedItems().count()/m_col==0)
        return;
    foreach(QTableWidgetSelectionRange range,m_selectionRange){
        for(int index=range.topRow();index<=range.bottomRow();index++){
            if(m_findTable->item(index,0)==NULL)
                break;
            if(m_dataList->at(index)->outTime=="" &&
                    m_parent->shopingFind(m_findTable->item(index,0)->text()) )//已出售或已添加
                continue;
            count++;
        }
    }
    //判断是否超过
    if(!m_parent->shopingCount(count)){
        m_parent->MessageHit("该单超过4个项目,无法添加");
        return;
    }
    if(count==0){
        m_parent->MessageHit(QString("选中的物品已添加或出售"));
        return;
    }
    //添加
    count=0;
    foreach(QTableWidgetSelectionRange range,m_selectionRange){
        for(int index=range.topRow();index<=range.bottomRow();index++){
            if(m_findTable->item(index,0)==NULL)
                break;
            if(m_model && (m_findTable->item(index,6)->text()!="未出售" || m_parent->shopingFind(m_findTable->item(index,0)->text())))//已出售或已添加
                continue;
            t_data = new PhoneData();
            (*t_data)=(*(m_dataList->at(index)));
            if(m_parent->addShopingItem(t_data))
                count++;
            delete t_data;
        }
    }
    if(count==0){
        m_parent->MessageHit(QString("选中的物品已添加或出售"));
        return;
    }
    if(m_model)
        m_loading->Start("SALE",m_editWidget->m_count,NULL,NULL,QStringList());
    m_parent->MessageHit(QString("已添加%1个物品到购物车").arg(count),800);
}

void PhoneWidget::ButtonAddPress(){//添加按钮
    chooseAddWidget();
}

void PhoneWidget::ButtonEditPress(){//修改按钮
    chooseEditWidget();
}

void PhoneWidget::ButtonDelPress(){//删除按钮
    chooseDelWidget();
}


/*----添加界面----*/
void PhoneWidget::createAddWidget(){//创建
    m_addWidget = new PhoneItemWidget(this,"ADD",m_model);
}

void PhoneWidget::chooseAddWidget(){//选择
    reAddWidget();
    m_addWidget->exec();
    //结束后处理
    if(m_addWidget->m_count>0 && m_findTable->item(0,0)!=NULL)
        ButtonFindPress();
}

void PhoneWidget::reAddWidget(){//重置
    m_addWidget->reWidget();
}

void PhoneWidget::ButtonAddOnePress(){//添加按钮
    //判断输入
    if(m_addWidget->m_IMEI->GetValue().length()!=15){
        m_data->SendMessageHit("串码应为15位的数字");
        return;
    }
    if(m_addWidget->m_IMEI->GetValue()=="000000000000000"){
        m_data->SendMessageHit("串码不能为000000000000000");
        return;
    }
    if(m_data->SearchDataBase("phoneDatabase.db","IMEI",m_addWidget->m_IMEI->GetValue())){
        m_data->SendMessageHit("串码数据库已录入");
        return;
    }
    if(m_addWidget->m_inTimeTimeBox->GetData()=="Empty"){
        m_data->SendMessageHit("请输入正确的入库时间");
        return;
    }
    //新建数据
    PhoneData *data = new PhoneData();
    data->IMEI=m_addWidget->m_IMEI->GetValue();//串码IMEI
    data->buyingPrice=m_addWidget->m_buyingPrice->GetValue().toFloat();//进货价
    data->salingPrice=m_addWidget->m_salingPrice->GetValue().toFloat();//销售价
    data->addition=m_addWidget->m_addition->GetValue();//备注
    data->phoneBrand=m_addWidget->m_phoneBrandCombBox->currentText();//手机品牌
    data->phoneModels=m_addWidget->m_phoneModelsCombBox->currentText();//手机型号
    data->inTime=m_addWidget->m_inTimeDateBox->GetData()+m_addWidget->m_inTimeTimeBox->GetData();//入库时间
    //提示框
    QString time=QString("%1时%2分%3秒").arg(m_addWidget->m_inTimeTimeBox->GetTime("hour"))
            .arg(m_addWidget->m_inTimeTimeBox->GetTime("min")).arg(m_addWidget->m_inTimeTimeBox->GetTime("sec"));
    QString date=QString("%1年%2月%3日").arg(m_addWidget->m_inTimeDateBox->GetDate("year"))
            .arg(m_addWidget->m_inTimeDateBox->GetDate("month")).arg(m_addWidget->m_inTimeDateBox->GetDate("day"));
    QString str=QString("是否添加一个数据？\n串码：%1\n手机品牌：%2\n手机型号：%3\n"
                        "进货价：%4元\n销售价：%5元\n入库时间：%6\n               %7\n")
                        .arg(data->IMEI).arg(data->phoneBrand).arg(data->phoneModels)
                        .arg(QString::number(data->buyingPrice,'f',2)).arg(QString::number(data->salingPrice,'f',2))
                        .arg(date).arg(time);
    if(data->addition!="")
        str+=QString("\n备注：%7").arg(data->addition);
    //添加
    if(m_parent->MessageQuestion(str)){
        if(m_data->InsertDatabase(data)){
            m_addWidget->m_count++;
            m_parent->MessageHit("添加成功",500);
            m_addWidget->m_IMEI->m_edit->setFocus();
        }
    }
    delete data;
}

void PhoneWidget::ButtonAddBackPress(){//返回按钮
    m_addWidget->close();
}

/*----编辑界面----*/
void PhoneWidget::createEditWidget(){//创建
    m_editWidget = new PhoneItemWidget(this,"EDIT",m_model);
}

void PhoneWidget::chooseEditWidget(){//选择
    int count=m_findTable->selectedItems().count()/m_col;
    if(count!=0){//批量修改
        reEditWidget();
    }
}

void PhoneWidget::reEditWidget(){//批量重置获取
    int same;
    int count;
    QString str;
    QString value[m_col];
    int i;
    //获取选中
    m_selectionRange=m_findTable->selectedRanges();
    //判断是否有相同项
    for(int j=1;j<m_col;j++)
        if(j<=5 || j>=9){
            same=-1;
            if(m_findTable->item(m_selectionRange.first().topRow(),0)==NULL)
                break;
            str=m_findTable->item(m_selectionRange.first().topRow(),j)->text();
            foreach(QTableWidgetSelectionRange range,m_selectionRange){
                for(i=range.topRow();i<=range.bottomRow();i++){
                    if(m_findTable->item(i,0)==NULL)
                        break;
                    if("未出售"==m_findTable->item(i,6)->text() && !m_parent->shopingFind(m_findTable->item(i,0)->text()) ){
                        if(same==-1){
                            str=m_findTable->item(i,j)->text();
                            same=1;
                        }
                        else if(str!=m_findTable->item(i,j)->text()){
                            same=0;
                            break;
                        }
                    }
                }
                if(same==0)
                    break;
            }
            if(same==1)
                value[j]=str;
            else
                value[j]="";
        }
    //记录串码
    count=0;
    m_editWidget->m_IMEI->m_edit->setEnabled(false);
    m_IDList.clear();
    foreach(QTableWidgetSelectionRange range,m_selectionRange){
        for(i=range.topRow();i<=range.bottomRow();i++){
            if(m_findTable->item(i,0)==NULL)
                break;
            if("未出售"==m_findTable->item(i,6)->text() && !m_parent->shopingFind(m_findTable->item(i,0)->text()) ){
                count++;
                m_IDList.append(m_findTable->item(i,0)->text());
            }
        }
    }
    //判读是否出售
    if(count==0){//已出售
        m_parent->MessageHit(QString("选中的物品已添加或出售"));
        return;
    }
    //询问是否批量修改
    if(count!=1){
        str=QString("是否批量修改%1个数据").arg(count);
        if(!m_parent->MessageQuestion(str))
            return;
    }
    //批量修改
    t_data = new PhoneData();
    //串码IMEI
    if(count==1){
        m_editWidget->m_IMEI->m_edit->setEnabled(true);
        t_data->IMEI=m_IDList.first();
    }
    else{
        m_editWidget->m_IMEI->m_edit->setEnabled(false);
        t_data->IMEI="";
    }
    t_data->phoneBrand=value[1];//手机品牌
    t_data->phoneModels=value[2];//手机型号
    t_data->buyingPrice=value[3].remove(0,1).toFloat();//入库价
    t_data->salingPrice=value[4].remove(0,1).toFloat();//销售价
    if(value[5]!="")
        t_data->inTime=value[5].remove(4,1).remove(6,1).remove(8,1).remove(10,1).remove(12,1);//时间
    else
        t_data->inTime="";
    t_data->addition=value[9];//备注

    m_editWidget->m_count=count;
    m_editWidget->reWidget(t_data);

    m_editWidget->exec();
    //结束后
    delete t_data;
}

void PhoneWidget::ButtonEditEnterPress(){//修改确定
    //判断输入
    if(!m_out && m_selectionRange.length()==1 && m_selectionRange.first().rowCount()==1){
        if(m_editWidget->m_IMEI->m_edit->isEnabled()){
            if(m_editWidget->m_IMEI->GetValue().length()!=15){
                m_data->SendMessageHit("串码应为15位的数字");
                return;
            }
            if(m_editWidget->m_IMEI->GetValue()=="000000000000000"){
                m_data->SendMessageHit("串码不能为000000000000000");
                return;
            }
            if(m_editWidget->m_IMEI->GetValue()!=t_data->IMEI && m_data->SearchDataBase("phoneDatabase.db","IMEI",m_editWidget->m_IMEI->GetValue())){
                m_data->SendMessageHit("串码数据库已录入");
                return;
            }
        }
        if(m_editWidget->m_inTimeDateBox->GetData()=="Empty" || m_editWidget->m_inTimeTimeBox->GetData()=="Empty"){
            m_data->SendMessageHit("请输入正确的入库时间");
           return;
        }
    }
    //新建数据
    PhoneData *data = new PhoneData();
    data->IMEI=m_editWidget->m_IMEI->GetValue();//串码IMEI
    data->buyingPrice=m_editWidget->m_buyingPrice->GetValue().toFloat();//进货价
    data->salingPrice=m_editWidget->m_salingPrice->GetValue().toFloat();//销售价
    data->addition=m_editWidget->m_addition->GetValue();//备注
    data->phoneBrand=m_editWidget->m_phoneBrandCombBox->currentText();//手机品牌
    data->phoneModels=m_editWidget->m_phoneModelsCombBox->currentText();//手机型号
    data->inTime=m_editWidget->m_inTimeDateBox->GetData()+m_editWidget->m_inTimeTimeBox->GetData();//入库时间
    //提示框
    QStringList strList;
    strList.clear();
    QString time=QString("%1时%2分%3秒").arg(m_editWidget->m_inTimeTimeBox->GetTime("hour"))
            .arg(m_editWidget->m_inTimeTimeBox->GetTime("min")).arg(m_editWidget->m_inTimeTimeBox->GetTime("sec"));
    QString date=QString("%1年%2月%3日").arg(m_editWidget->m_inTimeDateBox->GetDate("year"))
            .arg(m_editWidget->m_inTimeDateBox->GetDate("month")).arg(m_editWidget->m_inTimeDateBox->GetDate("day"));
    QString str;
    if(data->IMEI!=t_data->IMEI){
        strList.append("IMEI");
        str=QString("串码: %1->%2").arg(t_data->IMEI).arg(data->IMEI);
    }
    if(data->phoneBrand!=t_data->phoneBrand){
        strList.append("phoneBrand");
        str=QString("\n手机型号: %1->%2").arg(t_data->phoneBrand).arg(data->phoneBrand);
    }
    if(data->phoneModels!=t_data->phoneModels){
        strList.append("phoneModels");
        str+=QString("\n手机品牌: %1->%2").arg(t_data->phoneModels).arg(data->phoneModels);
    }
    if(data->buyingPrice!=t_data->buyingPrice){
        strList.append("buyingPrice");
        str+=QString("\n进货价   : ")+QString::number(t_data->buyingPrice,'f',2)+"->"+QString::number(data->buyingPrice,'f',2);
    }
    if(data->salingPrice!=t_data->salingPrice){
        strList.append("salingPrice");
        str+=QString("\n销售价   : ")+QString::number(t_data->salingPrice,'f',2)+"->"+QString::number(data->salingPrice,'f',2);
    }
    if(m_editWidget->m_inTimeDateBox->GetData()!="Empty" && m_editWidget->m_inTimeTimeBox->GetData()!="Empty"
            && data->inTime!=t_data->inTime){
        strList.append("inTime");
        time=QString("%1时%2分%3秒").arg(t_data->inTime.mid(8,2))
            .arg(t_data->inTime.mid(10,2)).arg(t_data->inTime.right(2));
        date=QString("%1年%2月%3日").arg(t_data->inTime.left(4))
            .arg(t_data->inTime.mid(4,2)).arg(t_data->inTime.mid(6,2));
        str+=QString("\n入库时间: %1\n               %2").arg(date).arg(time);
        time=QString("%1时%2分%3秒").arg(m_editWidget->m_inTimeTimeBox->GetTime("hour"))
            .arg(m_editWidget->m_inTimeTimeBox->GetTime("min")).arg(m_editWidget->m_inTimeTimeBox->GetTime("sec"));
        date=QString("%1年%2月%3日").arg(m_editWidget->m_inTimeDateBox->GetDate("year"))
            .arg(m_editWidget->m_inTimeDateBox->GetDate("month")).arg(m_editWidget->m_inTimeDateBox->GetDate("day"));
        str+=QString("\n               ->\n               %1\n               %2").arg(date).arg(time);
    }
    if(data->addition!=t_data->addition){
        strList.append("addition");
        if(t_data->addition=="")
            t_data->addition="空或不等";
        str+=QString("\n备注      : %1->%2").arg(t_data->addition).arg(data->addition);
    }
    if(str.length()==0){
        delete data;
        m_parent->MessageHit("未修改任何数据!");
        return;
    }

    if(m_selectionRange.length()==1 && m_selectionRange.first().rowCount()==1){//修改一个
        str=QString("是否修改%1?").arg(t_data->IMEI)+str;
    }
    else if(m_selectionRange.length()!=0)
        str=QString("是否批量修改?")+str;
    //修改
    if(m_parent->MessageQuestion(str)){
        //修改手机
        m_loading->Start("EDIT",m_editWidget->m_count,data,m_dataList,strList);//清界面
        if(m_out){
            m_data->Update("phoneDatabase.db","IMEI",m_IDList,strList,data);
            //修改购物车
            PhoneRecord *record=new PhoneRecord;
            record->IMEI=data->IMEI;
            record->phoneModels=data->phoneModels;
            record->buyingPrice=data->buyingPrice;
            record->salingPrice=data->salingPrice;
            record->count=1;
            record->unit="台";
            m_parent->ItemDetailsEditFinished("phone",record);
            delete record;
            m_editWidget->close();
            m_parent->MessageHit("已修改数据库和购物车的数据!");//成功
        }
        else{
            m_parent->m_loading->Start("Update","phoneDatabase.db","IMEI",m_IDList,strList,data);
            m_editWidget->close();
            m_parent->MessageHit("修改成功!");//成功
        }
    }
    delete data;
}

void PhoneWidget::ButtonEditBackPress(){//返回按钮
    m_editWidget->close();
}

/*----删除界面----*/
void PhoneWidget::chooseDelWidget(){
    QString str;
    //获取选中
    int count=m_findTable->selectedItems().count()/m_col;
    if(count==0)
        return;
    m_IDList.clear();
    m_selectionRange=m_findTable->selectedRanges();
    //记录串码
    count=0;
    m_IDList.clear();
    foreach(QTableWidgetSelectionRange range,m_selectionRange)
        for(int i=range.topRow();i<=range.bottomRow();i++){
            if(m_findTable->item(i,0)==NULL)
                break;
            if(!m_parent->shopingFind(m_findTable->item(i,0)->text()) ){
                count++;
                m_IDList.append(m_findTable->item(i,0)->text());
            }
            m_IDList.append(m_findTable->item(i,0)->text());
        }
    //判读是否出售
    if(count==0){//已出售
        m_parent->MessageHit(QString("选中的物品已添加到购物车，无法删除"));
        return;
    }
    //询问是否删除
    str=QString("是否删除%1个数据(添加到购物车的无法删除)").arg(count);
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
        //删手机
        m_parent->m_loading->Start("DeleteDatabase","phoneDatabase.db","IMEI",m_IDList);
        //清界面
        m_loading->Start("DEL",m_IDList.count(),NULL,m_dataList,m_IDList);

        while(m_findTable->rowCount()<10){
            m_findTable->insertRow(m_findTable->rowCount()-1);
        }
        m_findBtn2->setEnabled(false);
        if(m_model){
            m_findBtn4->setEnabled(false);
            m_findBtn5->setEnabled(false);
            m_result->setText("");
        }
        //成功
        m_parent->MessageHit("删除成功!");
        this->m_findEdit->setFocus();
        return;
    }
}

/*----详情界面----*/
void PhoneWidget::createDetailsWidget(){
    m_detailsWidget = new PhoneItemWidget(this,"DETAILS",m_model);
}

void PhoneWidget::chooseDetailsWidget(){
    //获取选中
    int count=m_findTable->selectedItems().count()/m_col;
    if(count!=1)
        return;
    int index=m_findTable->selectedRanges().first().topRow();
    //内容
    t_data = new PhoneData();
    (*t_data)=(*(m_dataList->at(index)));
    m_IDList.clear();
    m_IDList.append(t_data->IMEI);
    if(m_parent->shopingFind(t_data->IMEI))
        t_data->outTime="已添加";
    m_detailsWidget->m_count=1;
    m_detailsWidget->reWidget(t_data);

    m_detailsWidget->exec();
    delete t_data;
}
void PhoneWidget::ButtonDetailsPress(){
    chooseDetailsWidget();
}

void PhoneWidget::ButtonDetailsSaleAddPress(){
    if(!m_parent->shopingCount(1)){
        m_parent->MessageHit("该单超过4个项目,无法添加");
        return;
    }
    m_detailsWidget->close();
    if(m_parent->addShopingItem(t_data)){
        if(m_model)
            m_loading->Start("SALE",1,NULL,NULL,QStringList());
        m_parent->MessageHit(QString("已添加这个物品到购物车"),800);
    }
    else
        m_parent->MessageHit("添加失败!");
}

void PhoneWidget::ButtonDetailsSaleSumPress(){
    if(!m_parent->shopingCount(1)){
        m_parent->MessageHit("该单超过4个项目,无法添加");
        return;
    }
    m_detailsWidget->close();
    if(m_model)
        m_loading->Start("SALE",1,NULL,NULL,QStringList());
    m_parent->addShopingItemAndSum(t_data);
}

void PhoneWidget::ButtonDetailsBackPress(){
    m_detailsWidget->close();
}

void PhoneWidget::ButtonPrintPress(){
    PrintWidget widget(m_parent,m_findTable,"库存");
    widget.Start();
}

/*----键盘响应----*/
void PhoneWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return)){
        m_findBtn1->click();
    }
}
