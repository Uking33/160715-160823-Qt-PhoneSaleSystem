//窗口
#include "shopingwidget.h"
#include "mainwindow.h"
#include "toolitemwidget.h"
#include "printsum.h"
//布局
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
//控件
#include <QTableWidget>
#include <QPushButton>
#include "ubutton.h"
//数据
#include "data.h"
//辅助
#include <QTableWidgetSelectionRange>
#include <QHeaderView>

ShopingWidget::ShopingWidget(MainWindow *parent, Data *data, bool edit)
{
    m_model=edit;
    m_parent=parent;
    m_data=data;
    //---数据初始化---
    m_record = new PhoneRecordList();
    createWidget();
    reHitWidget();
}

ShopingWidget::~ShopingWidget(){
    delete m_layout;
    delete m_record;
}

//数据
int ShopingWidget::getCount(){
    return m_record->dataList->count();
}

int ShopingWidget::getToolCount(QString name){
    foreach(PhoneRecord *dd,(*m_record->dataList)){
        if(dd->phoneModels==name)
            return dd->count;
    }
    return 0;
}

bool ShopingWidget::containsData(const QString str){
    if(str.length()==15&&str.toLongLong()!=0){//手机
        foreach(PhoneRecord* dd,(*m_record->dataList)){
            if(dd->IMEI==str)
                return true;
        }
    }
    else{//配件
        foreach(PhoneRecord* dd,(*m_record->dataList)){
            if(dd->phoneModels==str)
                return true;
        }
    }
    return false;
}

bool ShopingWidget::insertData(PhoneData* data){
    if(containsData(data->IMEI))
        return false;
    PhoneRecord *record=new PhoneRecord;
    record->IMEI=data->IMEI;
    record->phoneModels=data->phoneModels;
    record->unit="台";
    record->count=1;
    record->buyingPrice=data->buyingPrice;
    record->salingPrice=data->salingPrice;
    m_record->dataList->append(record);
    m_btn1->setEnabled(true);
    m_btn4->setEnabled(false);
    m_btn5->setEnabled(true);
    updateTable();
    return true;
}

bool ShopingWidget::insertData(ToolData* data){
    if(containsData(data->name))
        return false;
    PhoneRecord *record=new PhoneRecord;
    record->IMEI="";
    record->phoneModels=data->name;
    record->unit=data->unit;
    record->count=data->count;
    record->buyingPrice=data->buyingPrice;
    record->salingPrice=data->salingPrice;
    m_record->dataList->append(record);

    m_btn1->setEnabled(true);
    m_btn4->setEnabled(false);
    m_btn5->setEnabled(true);
    updateTable();
    return true;
}

void ShopingWidget::clearData(){
    delete m_record;
    m_record = new PhoneRecordList();
    m_btn1->setEnabled(false);
    m_btn4->setEnabled(false);
    m_btn5->setEnabled(false);
    clearTable();
}

//窗口
void ShopingWidget::createWidget(){//创建
    m_layout = new QVBoxLayout();
    //---按钮---
    m_btnLayout = new QHBoxLayout();
    //创建按钮
    if(m_model){
        m_btn1 = new UButton("出售(Enter)","",100,50,m_btnLayout);
        m_btn2 = new UButton("配件(Insert)","Insert",100,50,m_btnLayout);
        m_btn3 = new UButton("修改(Space)","Space",100,50,m_btnLayout);
        m_btn4 = new UButton("删除(Del)","Delete",100,50,m_btnLayout);
        m_btn5 = new UButton("清空(Esc)","Esc",100,50,m_btnLayout);
    }
    else{
        m_btn1 = new UButton("出售(Enter)","",80,30,m_btnLayout);
        m_btn2 = new UButton("配件(Insert)","Insert",80,30,m_btnLayout);
        m_btn4 = new UButton("删除(Del)","Delete",80,30,m_btnLayout);
        m_btn5 = new UButton("清空(Esc)","Esc",80,30,m_btnLayout);
    }
    m_btn1->setEnabled(false);
    m_btn2->setEnabled(true);
    if(m_model) m_btn3->setEnabled(false);
    m_btn4->setEnabled(false);
    m_btn5->setEnabled(false);
    //信号连接
    connect(m_btn1,SIGNAL(clicked()),this,SLOT(ButtonPrintPress()));
    connect(m_btn2,SIGNAL(clicked()),this,SLOT(ButtonAddToolPress()));
    if(m_model) connect(m_btn3,SIGNAL(clicked()),this,SLOT(ButtonEditPress()));
    connect(m_btn4,SIGNAL(clicked()),this,SLOT(ButtonDelPress()));
    connect(m_btn5,SIGNAL(clicked()),this,SLOT(ButtonClearPress()));
    //---添加表格---
    QStringList headerLabels;
    int rows;
    rows=10;
    if(m_model){
        m_col=6;
        headerLabels<<  "手机型号/配件名称"<< "串码"<< "进货价"<< "销售价"<< "数量"<< "单位";
    }
    else{
        m_col=5;
        headerLabels<<  "手机型号/配件名称"<< "串码"<< "销售价"<< "数量"<< "单位";
    }
    //创建表格
    m_table=new QTableWidget(rows,m_col);
    m_table->setMaximumHeight(497);
    m_table->setMinimumHeight(497);
    //行标题
    m_table->setHorizontalHeaderLabels(headerLabels);
    m_table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_table->verticalHeader()->setDefaultSectionSize(47);//行高
    m_table->horizontalHeader()->setDefaultSectionSize(150);//列宽
    //设置对齐
    m_table->horizontalHeader()->setStretchLastSection(true);//右边界对齐
    m_table->verticalHeader()->setStretchLastSection(true);//下边界对齐
    //设置方式
    m_table->setSelectionBehavior (QAbstractItemView::SelectRows);//设置表格的选择方式
    m_table->setEditTriggers(false);//设置编辑方式
    m_table->setFocusPolicy(Qt::ClickFocus);

    connect(m_table,&QTableWidget::doubleClicked,[=](){
        if(m_table->item(m_table->currentRow(),0)!=NULL){
            PhoneRecord *data=m_record->dataList->at(m_table->currentRow());
            QString str;
            if(data->IMEI==""){
                str=data->phoneModels;
                m_parent->ItemDetails("tool",data);
            }
            else{
                str=data->IMEI;
                m_parent->ItemDetails("phone",str);
            }
        }
    });

    QObject::connect(m_table,&QTableWidget::itemSelectionChanged,[=](){
        if(m_model) m_btn3->setEnabled(true);
        m_btn4->setEnabled(true);
    });
    //---布局---
    if(m_model){
        m_btnLayout->setSpacing(30);
        m_layout->addStretch(1);
        m_layout->addLayout(m_btnLayout);
        m_layout->addStretch(1);
        m_layout->addWidget(m_table);
    }
    else{
        m_layout->addLayout(m_btnLayout);
        m_layout->addWidget(m_table);
    }
    this->setLayout(m_layout);
    if(m_model)
        m_parent->setSize(1100,650);
    else
        m_parent->setSize(900,600);
    //---提示框---
    m_hitlabel = new QLabel("");
    QFont font;
    font.setPointSize(10);
    m_hitlabel->setFont(font);
    m_hitEditlabel1 = new QLabel("营业员");
    m_hitEditlabel2 = new QLabel("收银员");
    m_hitEdit1 = new QComboBox();
    m_hitEdit2 = new QComboBox();
    m_hitbtn1 = new QPushButton("确定");
    m_hitbtn2 = new QPushButton("取消");
    m_hitlayout = new QGridLayout();
    m_hitwidget = new QDialog(this);    
    m_hitwidget->setWindowTitle("选择营业员/收银员");
    m_hitlayout->addWidget(m_hitlabel,0,0,1,4);
    m_hitlayout->addWidget(m_hitEditlabel1,1,0,1,1);
    m_hitlayout->addWidget(m_hitEdit1,1,1,1,3);
    m_hitlayout->addWidget(m_hitEditlabel2,2,0,1,1);
    m_hitlayout->addWidget(m_hitEdit2,2,1,1,3);
    m_hitlayout->addWidget(m_hitbtn1,3,0,1,2);
    m_hitlayout->addWidget(m_hitbtn2,3,2,1,2);
    m_hitwidget->setLayout(m_hitlayout);

    connect(m_hitbtn1,&QPushButton::clicked,this,&ShopingWidget::ButtonEnterPrintPress);
    connect(m_hitbtn2,&QPushButton::clicked,m_hitwidget,&QDialog::close);
}

void ShopingWidget::reHitWidget(){
    //添加人+选中人
    m_hitEdit1->clear();
    m_hitEdit2->clear();
    QStringList *strList;
    strList=m_data->GetString("staffDatabase.db","IDname");
    if(strList!=NULL){
        for(int i=0;i<strList->count();i++){
            m_hitEdit1->addItem(strList->at(i));
            m_hitEdit2->addItem(strList->at(i));
        }
        delete strList;
    }
    if(!m_model){
        m_hitEdit1->setCurrentText(m_parent->m_ID+m_parent->m_name);
        m_hitEdit2->setCurrentText(m_parent->m_ID+m_parent->m_name);
    }
}

void ShopingWidget::clearTable(){//重置
    int count;
    while((count=m_table->rowCount())>0)
        m_table->removeRow(count-1);
    for(int i=0;i<10;i++)
        m_table->insertRow(0);
}

void ShopingWidget::updateTable(){//更新表格
    clearTable();

    int i,j;
    QStringList *strList;
    if(m_model)
        strList=&Type::RECORD_ALL;
    else
        strList=&Type::RECORD_PART;
    i=0;
    //开始添加
    foreach(PhoneRecord *temp,(*m_record->dataList)){
        j=0;
        if(m_table->rowCount()<=i){
            m_table->insertRow(i);
            m_table->setRowHeight(i,47);
        }
        foreach(QString str,(*strList)){
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignCenter);
            item->setText(temp->GetData("show",str));
            m_table->setItem(i,j,item);
            j++;
        }
        i++;
    }
    if(m_table->itemAt(0,0)==NULL){
        m_btn1->setEnabled(false);
        m_btn4->setEnabled(false);
        m_btn5->setEnabled(false);
    }
    if(m_record->dataList->count()==4)
        m_btn2->setEnabled(false);
    else
        m_btn2->setEnabled(true);
}

//槽函数
void ShopingWidget::ButtonPrintPress(){//打印
    //判断+ID
    QString str;
    str=m_data->GetNextID("recordDatabase.db","ID",m_data->GetTimeDataStr().left(8));//单号
    if(str=="RangeUp"){
        m_parent->MessageHit("今日单数超过9999单");
        return;
    }
    m_record->ID=str;
    //对话框
    m_hitlabel->setText(QString("购物车有%1条项目,请选择").arg(getCount()));
    m_hitlabel->setAlignment(Qt::AlignCenter);
    m_btn1->setFocus();

    m_hitwidget->exec();
}

void ShopingWidget::ButtonEnterPrintPress(){//确定打印
    m_hitwidget->close();
    m_record->outTime=m_data->GetTimeDataStr();//出货时间
    m_record->shopAssistant=m_hitEdit1->currentText();//营业员
    m_record->cashier=m_hitEdit2->currentText();//收银员
    double sum;
    //售价
    sum=0;
    foreach(PhoneRecord *data,(*m_record->dataList)){
        sum+=data->salingPrice*data->count;
    }
    m_record->price=sum;
    //毛利
    sum=0;
    foreach(PhoneRecord *data,(*m_record->dataList)){
        sum+=data->buyingPrice*data->count;
    }
    m_record->gather=m_record->price-sum;

    m_parent->m_printsum->m_printer->setDocName("销售:"+m_record->ID);
    connect(m_parent->m_printsum,&PrintSum::PrintBegin,this,&ShopingWidget::ButtonEnterPrintPressFinished);
    m_parent->m_printsum->doPrint(m_record);
}

void ShopingWidget::ButtonEnterPrintPressFinished(){//开始打印
    disconnect(m_parent->m_printsum,&PrintSum::PrintBegin,this,&ShopingWidget::ButtonEnterPrintPressFinished);
    m_data->InsertDatabase(m_record);
    m_parent->shopingEnd();
    m_parent->MessageHit(QString("已出售:单号:%1。\n已添加到打印队列").arg(m_record->ID));
    ButtonClearPress();
}

void ShopingWidget::ButtonEditPress(){//修改
    //获取选中
    int count=m_table->selectedItems().count()/m_col;
    if(count==0)
        return;
    m_selectionRange=m_table->selectedRanges();
    //获取index
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
        m_parent->MessageHit("购物车的内容只能逐个修改!");
        return;
    }
    //判断种类
    if(m_table->item(index,1)->text()=="")//配件
        m_parent->ItemDetailsEdit("tool",m_record->dataList->at(index));
    else//手机
        m_parent->ItemDetailsEdit("phone",m_record->dataList->at(index));
}

void ShopingWidget::ButtonEditPressFinished(PhoneRecord *data){//修改结束
    //获取选中
    int count=m_table->selectedItems().count()/m_col;
    if(count==0)
        return;
    m_selectionRange=m_table->selectedRanges();
    //获取index
    int index=0;
    count=0;
    foreach(QTableWidgetSelectionRange range,m_selectionRange)
        for(int i=range.topRow();i<=range.bottomRow();i++){
            if(m_table->item(i,0)==NULL)
                break;
            index=i;
        }
    //赋值
    (*m_record->dataList->at(index))=(*data);
    updateTable();
}

void ShopingWidget::ButtonDelPress(){//删除
    //获取选中
    int count=m_table->selectedItems().count()/m_col;
    if(count==0)
        return;
    m_IDList.clear();
    m_selectionRange=m_table->selectedRanges();
    //记录串码    
    QStringList list1,list2;
    PhoneRecord* data;
    m_IDList.clear();
    foreach(QTableWidgetSelectionRange range,m_selectionRange)
        for(int i=range.topRow();i<=range.bottomRow();i++){
            if(m_table->item(i,0)==NULL)
                break;
            m_IDList.append(i);
            data=m_record->dataList->at(i);
            if(data->IMEI!="")
                list1.append(data->IMEI);
            else
                list2.append(data->phoneModels);
        }
    m_parent->shopingDel(list1,list2);
    //删除
    for(int i=m_IDList.count()-1;i>=0;i--)
        m_record->dataList->removeAt(m_IDList.at(i));
    //清界面
    updateTable();
}

void ShopingWidget::ButtonAddToolPress(){//添加配件
    QStringList *list1,*list2;
    list1=m_data->GetString("countDatabase.db","name");
    list2=m_data->GetString("countDatabase.db","count",false);
    QDialog dlg;
    QVBoxLayout layout;
    QHBoxLayout btnLayout;
    UButton btn1("添加","Return",60,20,&btnLayout);
    UButton btn2("取消","Esc",60,20,&btnLayout);
    connect(&btn1,&UButton::clicked,&dlg,&QDialog::accept);
    connect(&btn2,&UButton::clicked,&dlg,&QDialog::reject);
    QComboBox box;
    for(int i=0;i<list1->count();i++){
        if(!containsData(list1->at(i)) && list2->at(i).toInt()!=0){
            box.addItem(list1->at(i));
        }
    }
    delete list1;
    delete list2;
    layout.addWidget(&box);
    layout.addLayout(&btnLayout);
    dlg.setLayout(&layout);
    if(box.count()>0){
        if(dlg.exec()){
            m_parent->ItemShop("tool",box.currentText());
        }
    }
    else
        m_parent->MessageHit("无配件可添加!");
}

void ShopingWidget::ButtonClearPress(){//清除购物车
    //清其他界面
    QStringList list1,list2;
    foreach(PhoneRecord* data,(*m_record->dataList)){
        if(data->IMEI!="")
            list1.append(data->IMEI);
        else
            list2.append(data->phoneModels);
    }
    m_parent->shopingDel(list1,list2);
    //清数据和界面
    clearData();
}

/*----键盘响应----*/
void ShopingWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return)){
        m_btn1->click();
    }
}
