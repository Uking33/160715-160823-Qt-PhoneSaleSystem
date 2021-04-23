//窗口
#include "staffwidget.h"
#include "staffitemwidget.h"
#include "mainwindow.h"
#include "uloading.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
//控件
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include "ubutton.h"
#include "uline.h"
#include "udatebox.h"
#include "utimebox.h"
#include "urectbox.h"
//数据
#include "data.h"
//辅助
#include <QTableWidgetSelectionRange>
#include <QKeyEvent>
#include <QInputDialog>
#include <QComboBox>

StaffWidget::StaffWidget(MainWindow *parent, Data *data) : QWidget(parent){
    //初始化
    m_parent=parent;
    m_data=data;
    m_newFind=true;
    m_dataList=NULL;
    createWidget();
    //子界面
    m_addWidget = new StaffItemWidget(this,"ADD");
    m_editWidget = new StaffItemWidget(this,"EDIT");
    m_detailsWidget = new StaffItemWidget(this,"DETAILS");
}

StaffWidget::~StaffWidget(){
    clearTable();
    delete m_layout;
}

void StaffWidget::createWidget(){//创建
    m_layout = new QVBoxLayout();
    //---限定---
    m_findLayout = new QHBoxLayout();
    //起始时间
    m_outdateSLabel = new QLabel("业绩起始");
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
    //结束时间
    m_outdateELabel = new QLabel("业绩结束");
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
    //员工姓名
    m_nameLabel = new QLabel("员工姓名:");
    m_nameBox = new QComboBox();
    QStringList *strList;
    strList=m_data->GetString("staffDatabase.db","IDname");
    m_nameBox->addItem("全部");
    if(strList!=NULL){
        m_nameBox->addItems(*strList);
        delete strList;
    }
    m_nameBox->setCurrentIndex(0);
    QObject::connect(m_nameBox,&QComboBox::currentTextChanged,[=](){
       m_newFind=true;
    });
    m_nameLayout = new QHBoxLayout();
    m_nameLayout->addWidget(m_nameLabel);
    m_nameLayout->addWidget(m_nameBox);
    m_nameLayout->setSpacing(0);
    //员工工号
    m_findLabel = new QLabel("搜索:");
    m_edit = new QLineEdit();
    m_edit->setFocusPolicy(Qt::StrongFocus);
    m_edit->setValidator(new QIntValidator(0, 9999, m_edit));
    m_edit->setMaxLength(4);
    m_edit->setPlaceholderText("员工工号");
    connect(m_edit,&QLineEdit::textEdited,[=](){
        m_newFind=true;
    });
    m_editLayout = new QHBoxLayout();
    m_editLayout->addWidget(m_findLabel);
    m_editLayout->addWidget(m_edit);
    m_editLayout->setSpacing(10);
    //布局
    m_findLayout->setSpacing(50);
    m_findLayout->addLayout(m_outdateSLayout);
    m_findLayout->addLayout(m_nameLayout);
    m_findLayout->addLayout(m_editLayout);
    //---结果---
    m_result = new QLabel();
    m_result->setFixedSize(150,25);
    m_result->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    m_findLayout->addWidget(m_result);
    //---按钮---
    m_btnLayout = new QHBoxLayout();
    m_btnLayout->addLayout(m_outdateELayout);
    //创建按钮
    m_btnLayout->addStretch(1);
    m_btn1 = new UButton("查询(Enter)","",80,30,m_btnLayout);
    m_btnLayout->addStretch(1);
    m_btn2 = new UButton("添加(Insert)","Insert",80,30,m_btnLayout);
    m_btnLayout->addStretch(1);
    m_btn3 = new UButton("修改(Space)","Space",80,30,m_btnLayout);
    m_btnLayout->addStretch(1);
    m_btn4 = new UButton("删除(Delete)","Delete",80,30,m_btnLayout);
    m_btnLayout->addStretch(1);
    m_btn1->setEnabled(true);
    m_btn2->setEnabled(true);
    m_btn3->setEnabled(false);
    m_btn4->setEnabled(false);
    //信号连接
    connect(m_btn1,SIGNAL(clicked()),this,SLOT(ButtonFindPress()));
    connect(m_btn2,SIGNAL(clicked()),this,SLOT(ButtonAddPress()));
    connect(m_btn3,SIGNAL(clicked()),this,SLOT(ButtonEditPress()));
    connect(m_btn4,SIGNAL(clicked()),this,SLOT(ButtonDelPress()));
    m_btn1->setFocusPolicy(Qt::NoFocus);
    m_btn2->setFocusPolicy(Qt::NoFocus);
    m_btn3->setFocusPolicy(Qt::NoFocus);
    m_btn4->setFocusPolicy(Qt::NoFocus);
    //---添加表格---
    QStringList headerLabels;
    int rows,columns;
    rows=10;
    columns=9;
    headerLabels<< "工号"<< "姓名"<< "入职时间"<< "营业次数"<< "收银次数"<< "销售量"<<"销售额"<< "毛利"<< "备注";
    //创建表格
    m_table=new QTableWidget(rows,columns);
    m_table->setMaximumHeight(497);
    m_table->setMinimumHeight(497);
    //行标题
    m_table->setHorizontalHeaderLabels(headerLabels);
    m_table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_table->verticalHeader()->setDefaultSectionSize(47);//行高
    m_table->horizontalHeader()->setDefaultSectionSize(100);//列宽
    //设置对齐
    m_table->horizontalHeader()->setStretchLastSection(true);//右边界对齐
    m_table->verticalHeader()->setStretchLastSection(true);//下边界对齐
    //设置方式
    m_table->setSelectionBehavior ( QAbstractItemView::SelectRows);//设置表格的选择方式
    m_table->setEditTriggers(false);//设置编辑方式
    m_table->setFocusPolicy(Qt::ClickFocus);
    connect(m_table,&QTableWidget::doubleClicked,this,&StaffWidget::ButtonDetailsPress);
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

void StaffWidget::clearTable(){//重置
    //子界面
    if(m_detailsWidget->isVisible())
        m_detailsWidget->close();
    //表
    int count;
    while((count=m_table->rowCount())>0)
        m_table->removeRow(count-1);
    for(int i=0;i<10;i++)
        m_table->insertRow(0);
    m_result->setText("");
    //m_dataList
    if(m_dataList!=NULL){
        foreach(StaffDataList* data,(*m_dataList))
            delete data;
        delete m_dataList;
        m_dataList=NULL;
    }
}

void StaffWidget::ItemDetailsFindFinished(QList<StaffDataList *>* dataList){//外部强制查询-结束槽函数
    disconnect(m_data, &Data::SendStaffData, this, &StaffWidget::ItemDetailsFindFinished);
    if(dataList==NULL){
        return;
    }
    //内容
    StaffDataList *t_data;
    t_data = dataList->first();
    dataList->clear();
    delete dataList;
    m_detailsWidget->reWidget(t_data);
    m_detailsWidget->exec();
}

void StaffWidget::ItemDetailsFind(const QString ID){//外部强制查询
    //寻找
    connect(m_data, &Data::SendStaffData, this, &StaffWidget::ItemDetailsFindFinished,Qt::QueuedConnection);
    m_data->GetStaffDatabase(QString(" WHERE ID='%1'").arg(ID));
}

void StaffWidget::ButtonFindPressFinished(QList<StaffDataList *> *dataList){//获取数据到表格-结束槽函数
    disconnect(m_data, &Data::SendStaffData, this, &StaffWidget::ButtonFindPressFinished);
    m_dataList=dataList;
    m_btn2->setEnabled(false);
    m_btn3->setEnabled(false);
    m_btn4->setEnabled(false);
    if(dataList==NULL){
        m_result->setText("");
        delete dataList;
        return;
    }
    int i,j;
    QStringList *strList;
    strList=&Type::STAFF;
    //打印按钮
    m_btn2->setEnabled(true);
    m_btn4->setEnabled(true);
    //开始添加
    i=0;
    int count=0;
    QString time1=m_outdateSBox->GetData();
    QString time2=m_outdateEBox->GetData();
    foreach(StaffDataList *temp,(*dataList)){
        j=0;
        if(m_table->rowCount()<=i){
            m_table->insertRow(i);
            m_table->setRowHeight(i,47);
        }
        foreach(QString str,(*strList)){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            item->setText(temp->GetData("show",str,time1,time2));
            m_table->setItem(i,j,item);
            j++;
        }
        i++;
    }
    //结果
    m_newFind=false;
    count=i;
    m_result->setText(QString("搜索结果：%1条").arg(count));
}

void StaffWidget::ButtonFindPress(){//查询按钮
    //判断是否重复查询
    if(!m_newFind){
        QString time1=m_outdateSBox->GetData();
        QString time2=m_outdateEBox->GetData();
        QStringList strList;
        strList=Type::STAFF;
        int i=0,j=0;
        foreach(StaffDataList *temp,(*m_dataList)){
            j=0;
            foreach(QString str,(strList)){
                m_table->item(i,j)->setText(temp->GetData("show",str,time1,time2));
                j++;
            }
            i++;
        }
        return;
    }
    //清除表并连接
    clearTable();
    connect(m_data, &Data::SendStaffData, this, &StaffWidget::ButtonFindPressFinished,Qt::QueuedConnection);

    QString str1,str2;
    int count=0;
    str1=QString(" where(");
    //搜索框
    str2 = m_edit->text();
    str2=str2.simplified();
    if(str2=="")
        m_edit->setText("");
    else if(count>0)
        str1+=QString(" and ");
    if(str2!=""){//单号
        str1+=QString("%1 like '%%%2%%'").arg("ID").arg(str2);
        count++;
    }
    //名字
    str2 = m_nameBox->currentText();
    if(str2!="全部" && count>0)
        str1+=QString(" and ");
    if(str2!="全部"){
        str1+=QString("%1 like '%%%2%%'").arg("ID").arg(str2.left(4));
        count++;
    }
    //结尾
    str1+=")";
    if(str1==" where()")
        str1="";
    m_parent->m_loading->Start("GetStaffDatabase",str1);
}

void StaffWidget::ButtonAddPress(){//添加
    m_addWidget->reWidget();
    m_addWidget->exec();
}

void StaffWidget::ButtonAddPressEnter (){//添加新员工
    //判断
    if(m_addWidget->m_ID->GetValue().length()!=4){
        m_parent->MessageHit("工号要四位数字");
        return;
    }
    if(m_data->SearchDataBase("staffDatabase.db","ID",m_addWidget->m_ID->GetValue())){
        m_parent->MessageHit("工号已存在");
        return;
    }
    if(m_addWidget->m_name->GetValue().length()==0){
        m_parent->MessageHit("姓名不能为空");
        return;
    }
    //新建
    StaffDataList *staff=new StaffDataList();
    staff->ID=m_addWidget->m_ID->GetValue();
    staff->name=m_addWidget->m_name->GetValue();
    staff->inDate=m_addWidget->m_inTimeBox->GetData();
    staff->addition=m_addWidget->m_addition->GetValue();
    //提示框
    QString date=QString("%1年%2月%3日").arg(m_addWidget->m_inTimeBox->GetDate("year"))
            .arg(m_addWidget->m_inTimeBox->GetDate("month")).arg(m_addWidget->m_inTimeBox->GetDate("day"));
    QString str=QString("是否添加一个员工数据？\n工号：%1\n姓名：%2\n入职：%3")
                        .arg(staff->ID).arg(staff->name).arg(date);
    if(staff->addition!="")
        str+=QString("\n备注：%1").arg(staff->addition);
    if(m_parent->MessageQuestion(str)){
        //添加
        if(m_data->InsertDatabase(staff)){
            m_addWidget->close();
            m_parent->MessageHit(QString("添加成功!\n姓名:%1\n工号:%2\n密码:%2").arg(staff->name).arg(staff->ID));
            m_nameBox->addItem(staff->ID+staff->name);
            m_parent->updateStaff();
            m_newFind=true;
            ButtonFindPress();
        }
        else
            m_parent->MessageHit(QString("添加失败!"));
    }
    delete staff;
}

void StaffWidget::ButtonDelPress(){//删除按钮
    //获取选中
    int count=m_table->selectedItems().count()/9;
    if(count==0)
        return;
    m_IDList.clear();
    m_selectionRange=m_table->selectedRanges();
    //记录table
    m_IDList.clear();
    foreach(QTableWidgetSelectionRange range,m_selectionRange)
        for(int i=range.topRow();i<=range.bottomRow();i++){
            if(m_table->item(i,0)==NULL)
                break;
            if(m_table->item(i,0)->text()=="0000"){
                m_parent->MessageHit("删除管理员只能清空管理员的业绩和分摊信息");
            }
            m_IDList.append(m_table->item(i,0)->text()+m_table->item(i,1)->text());
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

        if(m_data->DeleteDatabase(m_IDList))
            m_parent->MessageHit("删除成功!");
        else
            m_parent->MessageHit("删除失败!");
        foreach(QString IDname,m_IDList){
            for(int i=0;i<m_nameBox->count();i++)
                if(m_nameBox->itemText(i)==IDname)
                    m_nameBox->removeItem(i);
        }
        m_parent->updateStaff();
        m_nameBox->setCurrentIndex(0);
        m_newFind=true;
        this->ButtonFindPress();
        connect(m_data, SIGNAL(SendMessageHit(QString)), m_parent, SLOT(MessageHit(QString)));
        return;
    }
}

void StaffWidget::ButtonEditPress(){//修改按钮
    //获取选中
    int count=m_table->selectedItems().count()/9;
    if(count==0)
        return;
    m_selectionRange=m_table->selectedRanges();
    //记录table
    int index=0;
    count=0;
    m_IDList.clear();
    foreach(QTableWidgetSelectionRange range,m_selectionRange)
        for(int i=range.topRow();i<=range.bottomRow();i++){
            if(m_table->item(i,0)==NULL)
                break;
            m_IDList.append("☺"+m_table->item(i,0)->text()+"☺"+m_table->item(i,1)->text()+"☺");
            index=i;
            count++;
        }
    if(count>1){
        m_parent->MessageHit("员工只能逐个修改!");
        return;
    }
    if(m_dataList->at(index)->ID=="0000"){
        bool ok;
        QInputDialog input;
        QString text;
        while(1){
            text=input.getText(this,"姓名输入","管理员，您好。请输入您的新姓名",QLineEdit::Normal,"",&ok);
            if (ok){
                if(text.isEmpty())
                    m_parent->MessageHit("姓名不能为空");
                else if(text=="管理员")
                    m_parent->MessageHit("姓名不能为管理员");
                else{
                    QStringList strList; strList.append("name");
                    StaffDataList* data=new StaffDataList; data->name=text; data->ID=m_parent->m_ID;
                    m_data->Update(m_parent->m_ID,m_parent->m_name,strList,data);
                    //清界面
                    for(int i=0;i<m_nameBox->count();i++)
                        if(m_nameBox->itemText(i)==m_parent->m_ID+m_parent->m_name)
                            m_nameBox->setItemText(i,data->ID+data->name);
                    m_parent->updateStaff();
                    m_dataList->at(index)->name=text;
                    m_table->item(index,1)->setText(text);
                    m_parent->m_name=text;
                    m_parent->setWindowTitle("手机管理系统 工号:"+m_parent->m_ID+" 姓名:"+m_parent->m_name);
                    m_parent->MessageHit("修改成功!");
                    delete data;
                    return;
                }
            }
            else{
                return;
            }
        }
    }
    //开启
    m_editWidget->reWidget(m_dataList->at(index));
    m_editWidget->exec();
}

void StaffWidget::ButtonEditPressEnter(){//修改结束
    //判断输入
    if(m_editWidget->m_ID->GetValue().length()!=4){
        m_parent->MessageHit("工号要四位数字");
        return;
    }
    else if(m_editWidget->m_record->ID!=m_editWidget->m_ID->GetValue() && m_data->SearchDataBase("staffDatabase.db","ID",m_editWidget->m_ID->GetValue())){
        m_parent->MessageHit("工号已存在");
        return;
    }
    else if(m_editWidget->m_name->GetValue().length()==0){
        m_parent->MessageHit("姓名不能为空");
        return;
    }
    //提示框
    StaffDataList *data=m_editWidget->m_record;
    QStringList strList;
    strList.clear();
    QString date;
    QString str;
    if(data->ID!=m_editWidget->m_ID->GetValue()){
        strList.append("ID");
        str=QString("\n工号(修改工号不会改变销售记录和分摊费用记录的工号): %1->%2").arg(data->ID).arg(m_editWidget->m_ID->GetValue());
    }
    if(data->name!=m_editWidget->m_name->GetValue()){
        strList.append("name");
        str=QString("\n姓名(修改名字不会改变销售记录和分摊费用记录的人名): %1->%2").arg(data->name).arg(m_editWidget->m_name->GetValue());
    }
    if(data->inDate!=m_editWidget->m_inTimeBox->GetData()){
        strList.append("inTime");
        date=QString("%1年%2月%3日").arg(data->inDate.left(4))
            .arg(data->inDate.mid(4,2)).arg(data->inDate.mid(6,2));
        str+=QString("\n入职时间: %1").arg(date);
        date=QString("%1年%2月%3日").arg(m_editWidget->m_inTimeBox->GetDate("year"),2,10,QChar('0'))
            .arg(m_editWidget->m_inTimeBox->GetDate("month"),2,10,QChar('0'))
                .arg(m_editWidget->m_inTimeBox->GetDate("day"),2,10,QChar('0'));
        str+=QString("\n               ->\n              %1").arg(date);
    }
    if(data->addition!=m_editWidget->m_addition->GetValue()){
        QString tempstr;
        strList.append("addition");
        tempstr=data->addition;
        if(tempstr=="")
            tempstr="空";
        str+=QString("\n备注      : %1").arg(tempstr);
        tempstr=m_editWidget->m_addition->GetValue();
        if(tempstr=="")
            tempstr="空";
        str+=QString("->%1").arg(tempstr);
    }

    if(str.length()==0){
        m_parent->MessageHit("未修改任何数据!");
        return;
    }
    str=QString("是否修改%1?").arg(m_editWidget->m_ID->GetValue())+str;

    //添加
    if(m_parent->MessageQuestion(str)){
        m_editWidget->close();
        //修改员工
        QString ID=data->ID;
        QString name=data->name;
        foreach (QString str, strList) {
            if(str=="ID")
                data->ID=m_editWidget->m_ID->GetValue();
            else if(str=="name")
                data->name=m_editWidget->m_name->GetValue();
            else if(str=="inTime")
                data->inDate=m_editWidget->m_inTimeBox->GetData();
            else if(str=="addition")
                data->addition=m_editWidget->m_addition->GetValue();
        }
        if(!m_data->Update(ID,name,strList,data)){
            m_parent->MessageHit("修改失败!");
            return;
        }
        //清界面
        for(int i=m_table->rowCount()-1;i>=0;i--){
            if(m_table->item(i,0)!=NULL && m_table->item(i,0)->text()==ID){//寻找
                foreach(QString str,strList){
                    if(str=="ID")
                        m_table->item(i,0)->setText(data->GetData("show","ID"));
                    else if(str=="name")
                        m_table->item(i,1)->setText(data->GetData("show","name"));
                    else if(str=="inTime")
                        m_table->item(i,2)->setText(data->GetData("show","inTime"));
                    else if(str=="addition")
                        m_table->item(i,8)->setText(data->GetData("show","addition"));
                }
                if(strList.contains("ID")||strList.contains("name")){
                    QString IDname=ID+name;
                    for(int i=0;i<m_nameBox->count();i++)
                        if(m_nameBox->itemText(i)==IDname)
                            m_nameBox->setItemText(i,data->ID+data->name);
                }
            }
        }
        m_parent->updateStaff();
        //成功
        m_parent->MessageHit("修改成功!");
    }
}

void StaffWidget::ButtonDetailsPress(){//详情
    //获取选中
    int count=m_table->selectedItems().count()/9;
    if(count!=1)
        return;
    int index=m_table->selectedRanges().first().topRow();
    //内容
    m_detailsWidget->reWidget(m_dataList->at(index));
    m_detailsWidget->exec();
}

/*----键盘响应----*/
void StaffWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return)){
        m_btn1->click();
    }
}
