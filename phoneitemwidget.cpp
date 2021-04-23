//窗口
#include "phoneitemwidget.h"
#include "phonewidget.h"
#include "data.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
//控件
#include <QLabel>
#include <QComboBox>
#include "ubutton.h"
#include "uline.h"
#include "udatebox.h"
#include "utimebox.h"
//数据
#include "type.h"
//辅助
#include <QLineEdit>
#include <QKeyEvent>

PhoneItemWidget::PhoneItemWidget(PhoneWidget *parent, const QString type, const bool model)
    :QDialog(parent)
{
    m_parent=parent;
    m_type=type;
    m_model=model;
    createWidget();
}

PhoneItemWidget::~PhoneItemWidget(){
    delete m_layout;
}

void PhoneItemWidget::createWidget(){
    m_layout = new QVBoxLayout(this);
    //---下侧控件---
    m_downLayout = new QVBoxLayout();
    //串码IMEI
    m_IMEI=new ULine("串码IMEI :","15位数字");
    m_IMEI->SetMaxLength(15);
    m_IMEI->m_type=2;
    m_IMEI->SetValidator("[0-9]{0,15}");
    //手机品牌
    m_phoneBrandLayout = new QHBoxLayout();
    m_phoneBrandLabel = new QLabel("手机品牌 :");
    m_phoneBrandCombBox = new QComboBox();
    m_phoneBrandLabel->setFixedSize(60,25);
    m_phoneBrandCombBox->setFixedSize(200,25);
    m_phoneBrandCombBox->setEditable(true);
    m_phoneBrandCombBox->lineEdit()->setMaxLength(30);
    m_phoneBrandCombBox->lineEdit()->setPlaceholderText("请输入手机品牌");
    m_phoneBrandLayout->addWidget(m_phoneBrandLabel);
    m_phoneBrandLayout->addWidget(m_phoneBrandCombBox);
    m_phoneBrandLayout->setAlignment(Qt::AlignLeft);
    connect(m_phoneBrandCombBox->lineEdit(),&QLineEdit::textEdited,[=](const QString str){
        if(str.contains("☺") || str.contains(",") || str.contains(" ") || str.contains("%"))
            m_phoneBrandCombBox->lineEdit()->backspace();
        for(int i=0;i<m_phoneBrandCombBox->count();i++)
            if(m_phoneBrandCombBox->itemText(i)==str){
                m_phoneBrandCombBox->setCurrentIndex(i);
                break;
            }
    });
    //手机型号
    m_phoneModelsLayout = new QHBoxLayout();
    m_phoneModelsLabel = new QLabel("手机型号 :");
    m_phoneModelsCombBox = new QComboBox();
    m_phoneModelsLabel->setFixedSize(60,25);
    m_phoneModelsCombBox->setFixedSize(200,25);
    m_phoneModelsCombBox->setEditable(true);
    m_phoneModelsCombBox->lineEdit()->setMaxLength(30);    
    m_phoneModelsCombBox->lineEdit()->setPlaceholderText("请输入手机型号");
    m_phoneModelsLayout->addWidget(m_phoneModelsLabel);
    m_phoneModelsLayout->addWidget(m_phoneModelsCombBox);
    m_phoneModelsLayout->setAlignment(Qt::AlignLeft);
    connect(m_phoneModelsCombBox->lineEdit(),&QLineEdit::textEdited,[=](const QString str){
        if(str.contains("☺") || str.contains(",") || str.contains(" ") || str.contains("%"))
            m_phoneModelsCombBox->lineEdit()->backspace();
        for(int i=0;i<m_phoneModelsCombBox->count();i++)
            if(m_phoneModelsCombBox->itemText(i)==str){
                m_phoneModelsCombBox->setCurrentIndex(i);
                break;
            }
    });
    //进货价
    if(m_model){
        m_buyingPrice=new ULine("进货价(¥):","请输入进货价");
        m_buyingPrice->SetDouble(6,2,999999);
    }
    //销售价
    m_salingPrice=new ULine("销售价(¥):","请输入销售价");
    m_salingPrice->SetDouble(6,2,999999);
    //入库时间
    m_inTimeLabel = new QLabel("入库时间 :\n\n");
    m_inTimeLabel->setFixedWidth(60);
    if(m_type=="ADD")
        m_inTimeDateBox = new UDateBox(1);
    if(m_type=="EDIT")
        m_inTimeDateBox = new UDateBox(0);
    if(m_type=="DETAILS"){
        m_inTimeDateBox = new UDateBox(0);
        QLineEdit *t_Line;
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_inTimeDateBox->m_comboBoxYear->setLineEdit(t_Line);
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_inTimeDateBox->m_comboBoxMonth->setLineEdit(t_Line);
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_inTimeDateBox->m_comboBoxDay->setLineEdit(t_Line);
    }
    m_inTimeDateBox->setFixedWidth(200);
    m_inTimeTimeBox = new UTimeBox();
    m_inTimeTimeBox->setFixedWidth(200);
    m_inTimeDetailsLayout = new QVBoxLayout();
    m_inTimeDetailsLayout->addWidget(m_inTimeDateBox);
    m_inTimeDetailsLayout->addWidget(m_inTimeTimeBox);
    m_inTimeLayout = new QHBoxLayout();
    m_inTimeLayout->addWidget(m_inTimeLabel);
    m_inTimeLayout->addLayout(m_inTimeDetailsLayout);
    m_inTimeLayout->setAlignment(Qt::AlignLeft);
    //备注
    m_addition=new ULine("    备注 :");
    m_addition->SetMaxLength(1000);
    //布局
    m_downLayout->addLayout(m_IMEI);
    m_downLayout->addLayout(m_phoneBrandLayout);
    m_downLayout->addLayout(m_phoneModelsLayout);
    if(m_model) m_downLayout->addLayout(m_buyingPrice);
    m_downLayout->addLayout(m_salingPrice);
    m_downLayout->addLayout(m_inTimeLayout);
    m_downLayout->addLayout(m_addition);
    //连接
    connect(m_IMEI,&ULine::textEdited,this,&PhoneItemWidget::TextEdit);
    connect(m_phoneBrandCombBox,&QComboBox::currentTextChanged,this,&PhoneItemWidget::TextEdit);
    connect(m_phoneModelsCombBox,&QComboBox::currentTextChanged,this,&PhoneItemWidget::TextEdit);
    if(m_model) connect(m_buyingPrice,&ULine::textEdited,this,&PhoneItemWidget::TextEdit);
    connect(m_salingPrice,&ULine::textEdited,this,&PhoneItemWidget::TextEdit);
    //---上侧控件---
    m_upLayout=new QHBoxLayout();
    if(m_type=="DETAILS"){
        m_btn1 = new UButton("添加(Enter)","",85,30,m_upLayout);
        m_btn2 = new UButton("添加并结帐","",85,30,m_upLayout);
        m_btn3 = new UButton("返回(Esc)","Esc",85,30,m_upLayout);
        m_btn1->setFont(QFont("Bold",10));
        m_btn2->setFont(QFont("Bold",10));
        m_btn3->setFont(QFont("Bold",10));
        m_btn1->setFocusPolicy(Qt::NoFocus);
        m_btn2->setFocusPolicy(Qt::NoFocus);
        m_btn3->setFocusPolicy(Qt::NoFocus);
        connect(m_btn1,SIGNAL(clicked()),m_parent,SLOT(ButtonDetailsSaleAddPress()));
        connect(m_btn2,SIGNAL(clicked()),m_parent,SLOT(ButtonDetailsSaleSumPress()));
        connect(m_btn3,SIGNAL(clicked()),m_parent,SLOT(ButtonDetailsBackPress()));
    }
    else if(m_type=="ADD"){
        m_btn1 = new UButton("添加(Enter)","",100,30,m_upLayout);
        m_btn2 = new UButton("返回(Esc)","Esc",100,30,m_upLayout);
        m_btn1->setFont(QFont("Bold",10));
        m_btn2->setFont(QFont("Bold",10));
        m_btn1->setFocusPolicy(Qt::NoFocus);
        m_btn2->setFocusPolicy(Qt::NoFocus);
        connect(m_btn1,SIGNAL(clicked()),m_parent,SLOT(ButtonAddOnePress()));
        connect(m_btn2,SIGNAL(clicked()),m_parent,SLOT(ButtonAddBackPress()));
    }
    else if(m_type=="EDIT"){
        m_btn1 = new UButton("修改(Enter)","",100,30,m_upLayout);
        m_btn2 = new UButton("取消(Esc)","Esc",100,30,m_upLayout);
        m_btn3 = NULL;
        m_btn1->setFont(QFont("Bold",10));
        m_btn2->setFont(QFont("Bold",10));
        m_btn1->setFocusPolicy(Qt::NoFocus);
        m_btn2->setFocusPolicy(Qt::NoFocus);
        connect(m_btn1,SIGNAL(clicked()),m_parent,SLOT(ButtonEditEnterPress()));
        connect(m_btn2,SIGNAL(clicked()),m_parent,SLOT(ButtonEditBackPress()));
    }

    //查看
    if(m_type=="DETAILS"){
        m_IMEI->m_edit->setReadOnly(true);
        m_phoneBrandCombBox->lineEdit()->setReadOnly(true);
        m_phoneModelsCombBox->lineEdit()->setReadOnly(true);
        if(m_model) m_buyingPrice->m_edit->setReadOnly(true);
        m_salingPrice->m_edit->setReadOnly(true);

        m_inTimeTimeBox->m_HourEdit->setReadOnly(true);
        m_inTimeTimeBox->m_MinEdit->setReadOnly(true);
        m_inTimeTimeBox->m_SecEdit->setReadOnly(true);
        m_addition->m_edit->setReadOnly(true);
    }
    //窗口
    setFixedSize(300,300);
    if(m_type=="ADD")
        setWindowTitle("添加新手机");
    if(m_type=="EDIT")
        setWindowTitle("修改手机信息");
    if(m_type=="DETAILS")
        setWindowTitle("查看手机信息");
    //---布局---
    m_layout->addLayout(m_upLayout);
    m_layout->addLayout(m_downLayout);
    setLayout(m_layout);
}
void PhoneItemWidget::TextEdit(){
    if(m_type=="EDIT" && m_count>1){
        if(m_phoneBrandCombBox->currentText()=="" || m_phoneModelsCombBox->currentText()==""
                || m_salingPrice->GetValue()=="" || (m_model && m_buyingPrice->GetValue()==""))
            m_btn1->setEnabled(false);
        else
            m_btn1->setEnabled(true);
    }
    else{
        if(m_IMEI->GetValue().length()!=15 || m_phoneBrandCombBox->currentText()=="" || m_phoneModelsCombBox->currentText()==""
                || m_salingPrice->GetValue()=="" || (m_model && m_buyingPrice->GetValue()==""))
            m_btn1->setEnabled(false);
        else
            m_btn1->setEnabled(true);
    }
}

//空白重置
void PhoneItemWidget::reWidget(){
    //串码
    m_IMEI->m_edit->setText("");
    //内容
    QStringList * list;
    //手机品牌+手机型号
    m_phoneBrandCombBox->clear();
    list=m_parent->m_data->GetString("phoneDatabase.db","phoneBrand");
    m_phoneBrandCombBox->addItems(*list);
    delete list;
    //手机型号
    m_phoneModelsCombBox->clear();
    list=m_parent->m_data->GetString("phoneDatabase.db","phoneModels");
    m_phoneModelsCombBox->addItems(*list);
    delete list;
    //入库价+销售价
    if(m_model) m_buyingPrice->SetValue("");
    m_salingPrice->SetValue("");
    //时间
    m_inTimeDateBox->Update();
    //emit m_inTimeDateBox->textChanged();
    m_inTimeTimeBox->Update();
    //备注
    m_addition->SetValue("");

    m_btn1->setEnabled(false);
    m_count=0;
}
//获取重置
void PhoneItemWidget::reWidget(PhoneData * data,bool isOut){
    //内容
    QStringList * list;
    //串码IMEI
    m_IMEI->SetValue(data->IMEI);
    //手机品牌
    m_phoneBrandCombBox->clear();
    if(m_type!="DETAILS" && !isOut){
        list=m_parent->m_data->GetString("phoneDatabase.db","phoneBrand");
        m_phoneBrandCombBox->addItems(*list);
        delete list;
    }
    m_phoneBrandCombBox->setCurrentText(data->phoneBrand);
    //手机型号
    m_phoneModelsCombBox->clear();
    if(m_type!="DETAILS" && !isOut){
        list=m_parent->m_data->GetString("phoneDatabase.db","phoneModels");
        m_phoneModelsCombBox->addItems(*list);
        delete list;
    }
    m_phoneModelsCombBox->setCurrentText(data->phoneModels);
    //入库价+销售价
    if(m_model) m_buyingPrice->SetValue(QString::number(data->buyingPrice));
    m_salingPrice->SetValue(QString::number(data->salingPrice));
    //时间
    if(m_type=="DETAILS"){
        if(isOut){
            QLineEdit *t_Line;
            t_Line=new QLineEdit();
            t_Line->setReadOnly(true);
            m_inTimeDateBox->m_comboBoxYear->setLineEdit(t_Line);
            t_Line=new QLineEdit();
            t_Line->setReadOnly(true);
            m_inTimeDateBox->m_comboBoxMonth->setLineEdit(t_Line);
            t_Line=new QLineEdit();
            t_Line->setReadOnly(true);
            m_inTimeDateBox->m_comboBoxDay->setLineEdit(t_Line);
        }
        else{
            QLineEdit *t_Line;
            t_Line=new QLineEdit();
            t_Line->setReadOnly(false);
            m_inTimeDateBox->m_comboBoxYear->setLineEdit(t_Line);
            t_Line=new QLineEdit();
            t_Line->setReadOnly(false);
            m_inTimeDateBox->m_comboBoxMonth->setLineEdit(t_Line);
            t_Line=new QLineEdit();
            t_Line->setReadOnly(false);
            m_inTimeDateBox->m_comboBoxDay->setLineEdit(t_Line);
        }
    }
    if(data->inTime==""){
        m_inTimeDateBox->Update("Empty");
        m_inTimeTimeBox->Update("Empty");
    }
    else{
        m_inTimeDateBox->Update(data->inTime.left(8));
        m_inTimeTimeBox->Update(data->inTime.right(6));
    }

    if(m_type=="DETAILS" || isOut){//清除处理
        m_inTimeDateBox->DisconnectBox();
        QString str;
        str=m_inTimeDateBox->m_comboBoxYear->currentText();
        m_inTimeDateBox->m_comboBoxYear->clear();
        if(isOut) m_inTimeDateBox->m_comboBoxYear->addItem(str);
        else m_inTimeDateBox->m_comboBoxYear->setCurrentText(str);
        str=m_inTimeDateBox->m_comboBoxMonth->currentText();
        m_inTimeDateBox->m_comboBoxMonth->clear();
        if(isOut) m_inTimeDateBox->m_comboBoxMonth->addItem(str);
        else m_inTimeDateBox->m_comboBoxMonth->setCurrentText(str);
        str=m_inTimeDateBox->m_comboBoxDay->currentText();
        m_inTimeDateBox->m_comboBoxDay->clear();
        if(isOut) m_inTimeDateBox->m_comboBoxDay->addItem(str);
        else m_inTimeDateBox->m_comboBoxDay->setCurrentText(str);
        m_inTimeDateBox->ConnectBox();
    }
    //备注
    m_addition->SetValue(data->addition);
    //按钮
    if(m_type=="DETAILS"){
        if(data->outTime!=""){
            m_btn1->setEnabled(false);
            m_btn2->setEnabled(false);
        }
        else{
            m_btn1->setEnabled(true);
            m_btn2->setEnabled(true);
        }
    }
    else{
        m_btn1->setEnabled(true);
        if(isOut){
            m_IMEI->m_edit->setReadOnly(true);
            m_phoneBrandCombBox->lineEdit()->setReadOnly(true);
            m_phoneModelsCombBox->lineEdit()->setReadOnly(true);
            if(m_model) m_buyingPrice->m_edit->setReadOnly(true);
            m_inTimeTimeBox->m_HourEdit->setReadOnly(true);
            m_inTimeTimeBox->m_MinEdit->setReadOnly(true);
            m_inTimeTimeBox->m_SecEdit->setReadOnly(true);
            m_salingPrice->SetFocus();
        }
        else{
            m_IMEI->m_edit->setReadOnly(false);
            m_phoneBrandCombBox->lineEdit()->setReadOnly(false);
            m_phoneModelsCombBox->lineEdit()->setReadOnly(false);
            if(m_model) m_buyingPrice->m_edit->setReadOnly(false);
            m_inTimeTimeBox->m_HourEdit->setReadOnly(false);
            m_inTimeTimeBox->m_MinEdit->setReadOnly(false);
            m_inTimeTimeBox->m_SecEdit->setReadOnly(false);
            m_IMEI->SetFocus();
        }
    }
    data->inTime=m_inTimeDateBox->GetData()+m_inTimeTimeBox->GetData();
}

void PhoneItemWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return)){
        if (m_type=="DETAILS" && (event->modifiers()==(Qt::ControlModifier)))
            m_btn2->click();
        else
            m_btn1->click();
    }
}
