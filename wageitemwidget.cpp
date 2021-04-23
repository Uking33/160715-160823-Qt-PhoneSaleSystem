//窗口
#include "wageitemwidget.h"
#include "wagewidget.h"
#include "mainwindow.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
//控件
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include "ubutton.h"
#include "uline.h"
#include "udatebox.h"
//数据
#include "data.h"
//辅助
#include <QDate>
#include <QKeyEvent>
#include <QValidator>
#include "uloading.h"

WageItemWidget::WageItemWidget(WageWidget *parent,QString type)
    :QDialog(parent)
{
    m_parent=parent;
    m_type=type;
    createWidget();
}

WageItemWidget::~WageItemWidget(){
    delete m_layout;
}
void WageItemWidget::createWidget(){
    m_layout = new QVBoxLayout(this);
    //---上侧控件---
    m_upLayout = new QGridLayout();
    //员工姓名
    m_nameLabel = new QLabel("员工姓名:");
    m_nameBox = new QComboBox();
    m_nameBox->setEditable(true);
    m_nameBox->lineEdit()->setMaxLength(30);
    m_nameBox->setFixedWidth(200);
    m_nameLayout = new QHBoxLayout();
    m_nameLayout->addWidget(m_nameLabel);
    m_nameLayout->addWidget(m_nameBox);
    m_nameLayout->setSpacing(0);
    //起始时间
    m_stratDateLabel=new QLabel("起始时间:");
    m_stratDateLabel->setFixedSize(60,25);
    if(m_type=="ADD")
        m_stratDateBox = new UDateBox(2);
    if(m_type=="DETAILS"){
        m_stratDateBox = new UDateBox(1);
        QLineEdit *t_Line;
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_stratDateBox->m_comboBoxYear->setLineEdit(t_Line);
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_stratDateBox->m_comboBoxMonth->setLineEdit(t_Line);
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_stratDateBox->m_comboBoxDay->setLineEdit(t_Line);
    }
    m_stratDateBox->setFixedSize(200,25);
    m_stratDate=new QHBoxLayout;
    m_stratDate->addWidget(m_stratDateLabel);
    m_stratDate->addWidget(m_stratDateBox);
    //截止时间
    m_endDateLabel=new QLabel("截止时间:");
    m_endDateLabel->setFixedSize(60,25);
    if(m_type=="ADD"){
        m_endDateBox = new UDateBox(3,m_stratDateBox);
        connect(m_stratDateBox,&UDateBox::textChanged,[=](){
            QString str=m_stratDateBox->GetData();
            if(str.left(6)==m_stratDateBox->m_dateMax->toString("yyyyMM")){
                m_endDateBox->Update(str.left(6)+QString("%1").arg(m_stratDateBox->m_dateMax->day(),2,10,QChar('0')));
            }
            else{                
                QDate date(m_stratDateBox->m_comboBoxYear->currentText().toInt(),m_stratDateBox->m_comboBoxMonth->currentText().toInt(),1);
                m_endDateBox->Update(str.left(6)+QString("%1").arg(date.daysInMonth(),2,10,QChar('0')));
            }
        });
        m_stratDateBox->ChooseDay("1");
    }
    if(m_type=="DETAILS"){
        m_endDateBox = new UDateBox(1);
        QLineEdit *t_Line;
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_endDateBox->m_comboBoxYear->setLineEdit(t_Line);
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_endDateBox->m_comboBoxMonth->setLineEdit(t_Line);
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_endDateBox->m_comboBoxDay->setLineEdit(t_Line);
    }
    m_endDateBox->setFixedSize(200,25);
    m_endDate=new QHBoxLayout;
    m_endDate->addWidget(m_endDateLabel);
    m_endDate->addWidget(m_endDateBox);
    //底薪
    m_base=new ULine("底薪(￥):","系统保留小数点后2位");
    connect(m_base->m_edit,QLineEdit::textEdited,[=](const QString str){
        if(str.length()==0)
            m_base->SetValue("0");
        if(str.length()==1){
            if(str==".")
                m_base->SetValue("0.");
            if(str=="+")
                m_base->SetValue("");
        }
        else if(str.length()==2){
            if(str=="00")
                m_base->SetValue("0");
            else if(!str.contains('.') && str.left(1)=="0")
                m_base->SetValue(str.right(1));
        }
    });
    QDoubleValidator *validator1 = new QDoubleValidator(0,999999,2, m_base->m_edit);
    validator1->setNotation(QDoubleValidator::StandardNotation);
    m_base->m_edit->setValidator(validator1);
    //提成
    m_percent=new ULine("提成(%):","系统保留小数点后1位");
    connect(m_percent->m_edit,QLineEdit::textEdited,[=](const QString str){
        if(str.length()==0)
            m_percent->SetValue("0");
        if(str.length()==1){
            if(str==".")
                m_percent->SetValue("0.");
            if(str=="+")
                m_percent->SetValue("");
        }
        else if(str.length()==2){
            if(str=="00")
                m_percent->SetValue("0");
            else if(!str.contains('.') && str.left(1)=="0")
                m_percent->SetValue(str.right(1));
        }
    });
    QDoubleValidator *validator2 = new QDoubleValidator(0,999,1, m_percent->m_edit);
    validator2->setNotation(QDoubleValidator::StandardNotation);
    m_percent->m_edit->setValidator(validator2);
    //布局
    m_upLayout->addLayout(m_nameLayout,0,0);
    m_upLayout->addLayout(m_base,1,0);
    m_upLayout->addLayout(m_percent,2,0);
    m_upLayout->addLayout(m_stratDate,3,0);
    m_upLayout->addLayout(m_endDate,4,0);

    if(m_type=="DETAILS"){
        //毛利
        m_gather=new ULine("毛利(￥):","不可输入");
        m_gather->m_edit->setReadOnly(true);
        m_gather->m_edit->setFocusPolicy(Qt::NoFocus);
        m_upLayout->addLayout(m_gather,5,0);
    }

    //右侧控件
    m_itemLayout = new QGridLayout();
    m_itemLayout->setSpacing(5);
    if(m_type=="ADD")
        m_itemLabel = new QLabel("   奖罚 \n\n\n\n\n");
    else if(m_type=="DETAILS")
        m_itemLabel = new QLabel("   奖罚 \n\n\n\n\n\n\n\n");
    m_itemLabel->setFixedWidth(50);
    m_itemList = new QListWidget();
    m_itemList->setFixedWidth(200);
    m_itemLayout->addWidget(m_itemLabel,0,0,1,1);
    m_itemLayout->addWidget(m_itemList,0,1,1,4);

    m_itemBtnLayout = new QHBoxLayout();
    QLabel *kong = new QLabel("");
    kong->setFixedSize(45,30);
    m_itemBtnLayout->addWidget(kong);
    m_itemBtn1 = new UButton("添(F1)","F1",45,30,m_itemBtnLayout);
    m_itemBtn2 = new UButton("修(F2)","F2",45,30,m_itemBtnLayout);
    m_itemBtn3 = new UButton("删(F3)","F3",45,30,m_itemBtnLayout);
    m_itemBtn4 = new UButton("重(F4)","F4",45,30,m_itemBtnLayout);
    m_itemBtn1->setFocusPolicy(Qt::NoFocus);
    m_itemBtn2->setFocusPolicy(Qt::NoFocus);
    m_itemBtn3->setFocusPolicy(Qt::NoFocus);
    m_itemBtn4->setFocusPolicy(Qt::NoFocus);
    if(m_type!="DETAILS"){
        connect(m_itemList,&QListWidget::itemDoubleClicked,[=](){
            ButtonEditPress();
        });
        connect(m_itemBtn1,SIGNAL(clicked()),this,SLOT(ButtonAddPress()));
        connect(m_itemBtn2,SIGNAL(clicked()),this,SLOT(ButtonEditPress()));
        connect(m_itemBtn3,SIGNAL(clicked()),this,SLOT(ButtonDelPress()));
        connect(m_itemBtn4,SIGNAL(clicked()),this,SLOT(ButtonClearPress()));
        QObject::connect(m_itemList,&QListWidget::itemSelectionChanged,[=](){
            m_itemBtn2->setEnabled(true);
            m_itemBtn3->setEnabled(true);
        });
    }
    else{
        connect(m_itemList,&QListWidget::itemDoubleClicked,[=](){
            ButtonDetailsPress();
        });
    }
    m_itemLayout->addLayout(m_itemBtnLayout,4,0,1,5);

    if(m_type=="ADD")
        m_upLayout->addLayout(m_itemLayout,0,1,5,1);
    else if(m_type=="DETAILS")
        m_upLayout->addLayout(m_itemLayout,0,1,6,1);

    //---下侧控件---
    m_downLayout = new QHBoxLayout();
    m_btn1 = new UButton("结算(Enter)","",120,40,m_downLayout);
    m_btn2 = new UButton("返回(Esc)","Esc",120,40,m_downLayout);
    m_btn1->setFocusPolicy(Qt::NoFocus);
    m_btn2->setFocusPolicy(Qt::NoFocus);
    m_btn1->setFont(QFont("Bold",10));
    m_btn2->setFont(QFont("Bold",10));
    if(m_type=="ADD"){
        connect(m_btn1,&UButton::clicked,this,&WageItemWidget::ButtonAddPressEnterPre);
        connect(m_btn2,&UButton::clicked,this,&WageItemWidget::close);
    }
    else if(m_type=="DETAILS"){
        m_btn1->setText("员工((Enter))");
        connect(m_btn1,&QPushButton::clicked,[=](){
            m_parent->m_parent->ItemDetails("staff",m_record->IDname.left(4));
        });
        connect(m_btn2,&QPushButton::clicked,this,&WageItemWidget::close);
    }
    //查看
    if(m_type=="DETAILS"){
        QLineEdit *t_Line;
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_nameBox->setLineEdit(t_Line);
        m_base->m_edit->setReadOnly(true);
        m_percent->m_edit->setReadOnly(true);
    }
    //提示框
    if(m_type!="DETAILS"){
        m_hitName = new ULine("名称","请输入奖罚名称");
        m_hitPrice = new ULine("金额(￥)","罚款输入负数,系统保留小数点后2位");
        m_hitReason = new ULine("原因","可输入奖罚原因");
        m_hitName->m_edit->setMaxLength(40);
        m_hitReason->m_edit->setMaxLength(40);
        connect(m_hitPrice->m_edit,QLineEdit::textEdited,[=](const QString str){
            if(str.length()==1){
                if(str==".")
                    m_hitPrice->SetValue("0.");
                if(str=="+")
                    m_hitPrice->SetValue("");
            }
            else if(str.length()==2){
                if(str=="00")
                    m_hitPrice->SetValue("0");
                else if(!str.contains('.') && str.left(1)=="0")
                    m_hitPrice->SetValue(str.right(1));
            }
        });
        QDoubleValidator *validator = new QDoubleValidator(-999999,999999,2, m_hitPrice->m_edit);
        validator->setNotation(QDoubleValidator::StandardNotation);
        m_hitPrice->m_edit->setValidator(validator);
        m_hitBtn1 = new QPushButton("确定");
        m_hitBtn2 = new QPushButton("取消");
        m_hitBtn1->setFocusPolicy(Qt::NoFocus);
        m_hitBtn2->setFocusPolicy(Qt::NoFocus);
        m_hitBtn1->setShortcut(QKeySequence("Return"));
        m_hitBtn2->setShortcut(QKeySequence("Esc"));
        m_hitBtn1->setEnabled(false);        
        connect(m_hitName->m_edit,&QLineEdit::textEdited,[=](){
            if(m_hitName->GetValue()==""||m_hitPrice->GetValue().toDouble()==0)
                m_hitBtn1->setEnabled(false);
            else
                m_hitBtn1->setEnabled(true);
        });
        connect(m_hitPrice->m_edit,&QLineEdit::textEdited,[=](){
            if(m_hitName->GetValue()==""||m_hitPrice->GetValue().toDouble()==0)
                m_hitBtn1->setEnabled(false);
            else
                m_hitBtn1->setEnabled(true);
        });
    }
    else{
        m_hitName = new ULine("名称","");
        m_hitPrice = new ULine("金额(￥)","");
        m_hitReason = new ULine("原因","");
        m_hitName->m_edit->setReadOnly(true);
        m_hitPrice->m_edit->setReadOnly(true);
        m_hitReason->m_edit->setReadOnly(true);
        m_hitBtn1 = new QPushButton("详情");
        m_hitBtn2 = new QPushButton("返回");
        connect(m_hitBtn1,&QPushButton::clicked,[=](){
                m_parent->m_parent->ItemDetails("error",m_record->rewardList.at(m_itemList->currentRow()).errorID);
        });
    }
    m_hitLayout = new QGridLayout();
    m_hitWidget = new QDialog(m_parent->m_parent);

    m_hitLayout->addLayout(m_hitName,0,0,1,2);
    m_hitLayout->addLayout(m_hitPrice,1,0,1,2);
    m_hitLayout->addLayout(m_hitReason,2,0,1,2);
    m_hitLayout->addWidget(m_hitBtn1,3,0);
    m_hitLayout->addWidget(m_hitBtn2,3,1);
    m_hitWidget->setLayout(m_hitLayout);

    connect(m_hitBtn1,&QPushButton::clicked,m_hitWidget,&QDialog::accept);
    connect(m_hitBtn2,&QPushButton::clicked,m_hitWidget,&QDialog::close);
    //窗口
    if(m_type=="ADD"){
        setFixedSize(550,200);
        setWindowTitle("结算员工工资");
    }
    else if(m_type=="DETAILS"){
        setFixedSize(550,250);
        setWindowTitle("查看员工工资");
    }
    else if(m_type=="EDIT")
        setWindowTitle("修改员工工资");
    //---布局---
    m_layout->addLayout(m_downLayout,1);
    m_layout->addLayout(m_upLayout,3);
    setLayout(m_layout);
}

//空白重置
void WageItemWidget::reWidget(){
    m_nameBox->clear();
    QStringList *strList;
    strList=m_parent->m_data->GetString("staffDatabase.db","IDname");
    if(strList!=NULL){
        foreach(QString str,(*strList))
            m_nameBox->addItem(str);
        delete strList;
    }
    m_stratDateBox->ToMonth(true);
    m_nameBox->setCurrentText("");
    m_base->SetValue("0");
    m_percent->SetValue("0");
    m_itemList->clear();
    m_itemBtn1->setEnabled(true);
    m_itemBtn2->setEnabled(false);
    m_itemBtn3->setEnabled(false);
    m_itemBtn4->setEnabled(false);
    m_hitName->SetValue("");
    m_hitPrice->SetValue("");
    m_hitReason->SetValue("");
}
//获取重置
void WageItemWidget::reWidget(WageData * data){
    m_record=data;
    m_nameBox->setCurrentText(data->IDname);
    m_stratDateBox->Update(data->startDate);
    m_endDateBox->Update(data->endDate);
    m_base->SetValue(QString::number(data->base,'f',2));
    m_gather->SetValue(QString::number(data->gather,'f',2));
    m_percent->SetValue(QString::number(data->per,'f',1));
    m_itemList->clear();
    m_itemList->addItems(data->GetListItem());
    //清除处理
    QString str;
    m_stratDateBox->DisconnectBox();
    str=m_stratDateBox->m_comboBoxYear->currentText();
    m_stratDateBox->m_comboBoxYear->clear();
    m_stratDateBox->m_comboBoxYear->setCurrentText(str);
    str=m_stratDateBox->m_comboBoxMonth->currentText();
    m_stratDateBox->m_comboBoxMonth->clear();
    m_stratDateBox->m_comboBoxMonth->setCurrentText(str);
    str=m_stratDateBox->m_comboBoxDay->currentText();
    m_stratDateBox->m_comboBoxDay->clear();
    m_stratDateBox->m_comboBoxDay->setCurrentText(str);

    m_endDateBox->DisconnectBox();
    str=m_endDateBox->m_comboBoxYear->currentText();
    m_endDateBox->m_comboBoxYear->clear();
    m_endDateBox->m_comboBoxYear->setCurrentText(str);
    str=m_endDateBox->m_comboBoxMonth->currentText();
    m_endDateBox->m_comboBoxMonth->clear();
    m_endDateBox->m_comboBoxMonth->setCurrentText(str);
    str=m_endDateBox->m_comboBoxDay->currentText();
    m_endDateBox->m_comboBoxDay->clear();
    m_endDateBox->m_comboBoxDay->setCurrentText(str);
    //按钮
    m_itemBtn1->setEnabled(false);
    m_itemBtn2->setEnabled(false);
    m_itemBtn3->setEnabled(false);
    m_itemBtn4->setEnabled(false);
}

void WageItemWidget::ButtonAddPressEnterPre(){
    //判断
    if(m_nameBox->currentText()==""){
        m_parent->m_parent->MessageHit("员工姓名不能为空");
        return;
    }
    connect(m_parent->m_data, &Data::SendStaffData, m_parent, &WageWidget::ButtonAddPressEnter,Qt::QueuedConnection);
    m_parent->m_parent->m_loading->Start("GetStaffDatabase",QString(" where ID='%1'").arg(m_nameBox->currentText().left(4)),QString(" where (date>='%1' and date<='%2') or unSale='21'").arg(m_stratDateBox->GetData()).arg(m_endDateBox->GetData()));
}

//槽函数按键
void WageItemWidget::ButtonAddPress(){
    if(m_itemList->count()==100){
        m_parent->m_parent->MessageHit("已添满100个奖罚项目");
        return;
    }
    //重置
    m_hitName->SetValue("");
    m_hitPrice->SetValue("0");
    //选择添加
    if(m_hitWidget->exec()){
        //修改显示
        m_itemList->addItem(m_hitName->GetValue()+":"+QString::number(m_hitPrice->GetValue().toDouble(),'f',2)+QString("元 %1").arg(m_hitReason->GetValue()));
        m_itemBtn4->setEnabled(true);
        //是否能再添加
        if(m_itemList->count()==100)
            m_itemBtn1->setEnabled(false);
    }
}
void WageItemWidget::HitNameChanged(QString){
    if(m_hitName->GetValue()==""||m_hitPrice->GetValue().toDouble()==0)
        m_hitBtn1->setEnabled(false);
    else
        m_hitBtn1->setEnabled(true);
}
void WageItemWidget::HitPriceChanged(QString){
    if(m_hitName->GetValue()==""||m_hitPrice->GetValue().toDouble()==0)
        m_hitBtn1->setEnabled(false);
    else
        m_hitBtn1->setEnabled(true);
}

void WageItemWidget::ButtonEditPress(){
    //重置
    QString str;
    int index=m_itemList->currentRow();
    str=m_itemList->item(index)->text();
    m_hitName->SetValue(str.section(" ",0,0));
    str=str.section(" ",1,1);
    m_hitPrice->SetValue(str.remove(str.length()-1,1));
    m_hitReason->SetValue(str.section(" ",2,2));
    m_hitPrice->m_edit->setFocus();
    //是否修改
    if(m_hitWidget->exec()){
        //修改显示
        m_itemList->currentItem()->setText(m_hitName->GetValue()+":"+QString::number(m_hitPrice->GetValue().toDouble(),'f',2)+QString("元 %1").arg(m_hitReason->GetValue()));
    }
}

void WageItemWidget::ButtonDelPress(){
    //删除显示
    QListWidgetItem *item = m_itemList->currentItem();
    m_itemList->removeItemWidget(item);
    delete item;
    m_itemBtn1->setEnabled(true);
    m_itemBtn2->setEnabled(false);
    m_itemBtn3->setEnabled(false);
    m_itemBtn4->setEnabled(false);
}

void WageItemWidget::ButtonClearPress(){
    //删除显示
    QListWidgetItem *item;
    for(int i=m_itemList->count()-1;i>=0;i--){
        item = m_itemList->item(i);
        m_itemList->removeItemWidget(item);
        delete item;
    }
    m_itemBtn1->setEnabled(true);
    m_itemBtn2->setEnabled(false);
    m_itemBtn3->setEnabled(false);
    m_itemBtn4->setEnabled(false);
}

void WageItemWidget::ButtonDetailsPress(){
    //重置
    QString str;
    int index=m_itemList->currentRow();
    str=m_itemList->item(index)->text();
    m_hitName->SetValue(m_record->rewardList.at(index).name);
    m_hitPrice->SetValue(QString::number(m_record->rewardList.at(index).price,'f',2));
    m_hitReason->SetValue(m_record->rewardList.at(index).reason);
    if(m_record->rewardList.at(index).errorID!="")
        m_hitBtn1->setEnabled(true);
    else
        m_hitBtn1->setEnabled(false);
    //是否修改
    m_hitWidget->exec();
}

/*----键盘响应----*/
void WageItemWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return))
        m_btn1->click();
}
