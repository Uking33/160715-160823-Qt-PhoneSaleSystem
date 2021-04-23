//窗口
#include "erroritemwidget.h"
#include "errorwidget.h"
#include "mainwindow.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
//控件
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include "ubutton.h"
#include "uline.h"
#include "udatebox.h"
//数据
#include "data.h"
//辅助
#include <QKeyEvent>
#include <QComboBox>

ErrorItemWidget::ErrorItemWidget(ErrorWidget *parent,QString type)
    :QDialog(parent)
{
    m_parent=parent;
    m_type=type;
    createWidget();
}

ErrorItemWidget::~ErrorItemWidget(){
    //---上边布局---
    delete m_upLayout;
    //---下边布局---
    delete m_btn1;
    delete m_btn2;
    delete m_downLayout;

    delete m_layout;
}
void ErrorItemWidget::createWidget(){
    m_layout = new QVBoxLayout(this);
    //---上侧控件---
    m_upLayout = new QGridLayout();
    m_name=new ULine("项目名称:","请输入项目名称");
    m_dateLabel=new QLabel("分摊时间:");
    m_dateLabel->setFixedSize(60,25);
    if(m_type=="ADD")
        m_dateBox = new UDateBox(1);
    if(m_type=="EDIT")
        m_dateBox = new UDateBox(0);
    if(m_type=="DETAILS"){
        m_dateBox = new UDateBox(0);
        QLineEdit *t_Line;
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_dateBox->m_comboBoxYear->setLineEdit(t_Line);
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_dateBox->m_comboBoxMonth->setLineEdit(t_Line);
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_dateBox->m_comboBoxDay->setLineEdit(t_Line);
    }
    m_dateBox->setFixedSize(200,25);
    m_date=new QHBoxLayout;
    m_date->addWidget(m_dateLabel);
    m_date->addWidget(m_dateBox);
    m_price=new ULine("总金额(￥):","系统保留小数点后2位");
    m_reason=new ULine("分摊原因:","可输入分摊原因");

    m_name->SetMaxLength(50);
    m_price->SetDouble(2,999999);
    m_reason->SetMaxLength(1000);

    m_upLayout->addLayout(m_name,0,0);
    m_upLayout->addLayout(m_date,1,0);
    m_upLayout->addLayout(m_price,2,0);
    m_upLayout->addLayout(m_reason,3,0);
    //右侧控件
    m_itemLayout = new QGridLayout();
    m_itemLayout->setSpacing(5);
    m_itemLabel = new QLabel("   员工 \n\n\n\n\n");
    m_itemLabel->setFixedWidth(50);
    m_itemList = new QListWidget();
    m_itemList->setFixedSize(200,100);
    m_itemLayout->addWidget(m_itemLabel,0,0,1,1);
    m_itemLayout->addWidget(m_itemList,0,1,1,4);

    m_itemBtnLayout = new QHBoxLayout();
    QLabel *kong = new QLabel("");
    kong->setFixedSize(45,30);
    m_itemBtnLayout->addWidget(kong);
    m_itemBtn1 = new UButton("添(F1)","F1",45,30,m_itemBtnLayout);
    m_itemBtn2 = new UButton("修(F2)","F2",45,30,m_itemBtnLayout);
    m_itemBtn3 = new UButton("删(F3)","F3",45,30,m_itemBtnLayout);
    m_itemBtn4 = new UButton("清(F4)","F4",45,30,m_itemBtnLayout);
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
    m_itemLayout->addLayout(m_itemBtnLayout,4,0,1,5);
    m_upLayout->addLayout(m_itemLayout,0,1,4,1);
    //---下侧控件---
    m_downLayout = new QHBoxLayout();
    if(m_type=="DETAILS")
        m_btn1 = new UButton("确定(←)","Backspace",120,40,m_downLayout);
    else
        m_btn1 = new UButton("确定(Enter)","",120,40,m_downLayout);
    m_btn2 = new UButton("返回(Esc)","Esc",120,40,m_downLayout);
    m_btn1->setFocusPolicy(Qt::NoFocus);
    m_btn2->setFocusPolicy(Qt::NoFocus);
    m_btn1->setFont(QFont("Bold",10));
    m_btn2->setFont(QFont("Bold",10));
    if(m_type=="ADD"){
        connect(m_btn1,&UButton::clicked,m_parent,&ErrorWidget::ButtonAddPressEnter);
        connect(m_btn2,&UButton::clicked,this,&ErrorItemWidget::close);
    }
    else if(m_type=="DETAILS"){
        connect(m_btn1,&UButton::clicked,m_parent,&ErrorWidget::DetailsWidgetUnsale);
        connect(m_btn2,&QPushButton::clicked,this,&ErrorItemWidget::close);
    }
    else if(m_type=="EDIT"){
        connect(m_btn1,&QPushButton::clicked,m_parent,&ErrorWidget::ButtonEditPressEnter);
        connect(m_btn2,&QPushButton::clicked,this,&ErrorItemWidget::close);
    }
    //查看
    if(m_type=="DETAILS"){
        m_name->m_edit->setReadOnly(true);
        m_price->m_edit->setReadOnly(true);
        m_reason->m_edit->setReadOnly(true);
    }
    //提示框
    if(m_type!="DETAILS"){
        //修改BTN1
        connect(m_price->m_edit,&QLineEdit::textEdited,[=](QString str){
            double p=str.toDouble();
            double sum=0;
            QString str1;
            for(int i=0;i<m_itemList->count();i++){
                str1=m_itemList->item(i)->text().section("  ",2,2);
                sum+=str1.remove(0,1).toDouble();
            }
            if(p<sum){
                m_itemBtn1->setEnabled(false);
            }
            else if(p==sum)
                m_itemBtn1->setEnabled(false);
            else{
                if(p==0)
                    m_itemBtn1->setEnabled(false);
                else
                    m_itemBtn1->setEnabled(true);
            }
        });
        //修改列表和price
        connect(m_price->m_edit,&QLineEdit::editingFinished,[=](){
            double p=m_price->m_edit->text().toDouble();
            double sum=0;
            QString str1,str2,str3;
            for(int i=0;i<m_itemList->count();i++){
                str1=m_itemList->item(i)->text().section("  ",2,2);
                sum+=str1.remove(0,1).toDouble();
            }
            //修改price
            if(p<sum){
                m_price->SetValue(QString::number(sum));
                m_parent->m_parent->MessageHit("请检查右侧员工列表再进行总金额修改");
                m_itemBtn1->setEnabled(false);
            }
            else if(p==sum){
                if(m_hitStaffName->findText(m_hitStaffName->currentText())==-1||m_per->GetValue().toDouble()>m_max||m_per->GetValue().toDouble()==0
                        ||m_staffCount==m_itemList->count())
                    m_itemBtn1->setEnabled(false);
            }
            else
                m_itemBtn1->setEnabled(true);
            //修改百分比
            p=m_price->m_edit->text().toDouble();
            for(int i=0;i<m_itemList->count();i++){
                str1=m_itemList->item(i)->text().section("  ",0,0);
                str3=m_itemList->item(i)->text().section("  ",2,2);
                str2=QString::number(str3.remove(0,1).toDouble()/p*100,'f',1)+"%";
                m_itemList->item(i)->setText(str1+"  "+str2+"  ¥"+str3);
            }
        });
        m_hitLabel = new QLabel("人员");
        m_hitLabel->setFixedWidth(60);
        m_hitStaffName = new QComboBox();
        QLineEdit *toolText = new QLineEdit();
        toolText->setMaxLength(30);
        m_hitStaffName->setLineEdit(toolText);
        m_per = new ULine("金额(￥)","系统保留小数点后2位");
        m_per->m_edit->setFixedWidth(140);
        m_per->SetDouble(2,999999);
        m_hitBtn1 = new QPushButton("确定");
        m_hitBtn2 = new QPushButton("取消");
        m_hitBtn1->setFocusPolicy(Qt::NoFocus);
        m_hitBtn2->setFocusPolicy(Qt::NoFocus);
        m_hitBtn1->setShortcut(QKeySequence("Return"));
        m_hitBtn2->setShortcut(QKeySequence("Esc"));
        m_hitBtn1->setEnabled(false);

        m_hitLayout = new QGridLayout();
        m_hitWidget = new QDialog(m_parent->m_parent);

        m_hitLayout->addWidget(m_hitLabel,0,0);
        m_hitLayout->addWidget(m_hitStaffName,0,1,1,3);
        m_hitLayout->addLayout(m_per,1,0,1,4);
        m_hitLayout->addWidget(m_hitBtn1,2,0,1,2);
        m_hitLayout->addWidget(m_hitBtn2,2,2,1,2);
        m_hitWidget->setLayout(m_hitLayout);

        connect(m_hitBtn1,&QPushButton::clicked,m_hitWidget,&QDialog::accept);
        connect(m_hitBtn2,&QPushButton::clicked,m_hitWidget,&QDialog::close);
        connect(m_hitStaffName->lineEdit(),&QLineEdit::textEdited,[=](const QString str){
            if(str.contains("☺") || str.contains(",") || str.contains(" ") || str.contains("%"))
                m_hitStaffName->lineEdit()->backspace();
        });
    }
    //窗口
    setFixedSize(550,200);
    if(m_type=="ADD")
        setWindowTitle("添加分摊项目");
    else if(m_type=="DETAILS")
        setWindowTitle("查看分摊项目");
    else if(m_type=="EDIT")
        setWindowTitle("修改分摊项目");
    //---布局---
    m_layout->addLayout(m_downLayout,1);
    m_layout->addLayout(m_upLayout,3);
    setLayout(m_layout);
}
//空白重置
void ErrorItemWidget::reWidget(){
    m_name->SetValue("");
    m_dateBox->m_state=1;
    m_dateBox->Update();
    m_price->SetValue("");
    m_reason->SetValue("");
    m_itemList->clear();
    m_itemBtn1->setEnabled(false);
    m_itemBtn2->setEnabled(false);
    m_itemBtn3->setEnabled(false);
    m_itemBtn4->setEnabled(false);
    m_per->SetValue("");
    m_hitStaffName->setCurrentText("");
}
//获取重置
void ErrorItemWidget::reWidget(ErrorData * data){
    m_record=data;
    m_name->SetValue(data->name);
    m_dateBox->Update(data->date);
    m_price->SetValue(QString::number(data->price));
    m_reason->SetValue(data->reason);
    m_itemList->clear();
    m_itemList->addItems(data->GetListItem());
    if(m_type=="DETAILS"){//清除处理
        m_dateBox->DisconnectBox();
        QString str;
        str=m_dateBox->m_comboBoxYear->currentText();
        m_dateBox->m_comboBoxYear->clear();
        m_dateBox->m_comboBoxYear->setCurrentText(str);
        str=m_dateBox->m_comboBoxMonth->currentText();
        m_dateBox->m_comboBoxMonth->clear();
        m_dateBox->m_comboBoxMonth->setCurrentText(str);
        str=m_dateBox->m_comboBoxDay->currentText();
        m_dateBox->m_comboBoxDay->clear();
        m_dateBox->m_comboBoxDay->setCurrentText(str);
        m_dateBox->ConnectBox();
    }
    //按钮
    if(m_type=="DETAILS"){
        if(data->unSale=="00" || data->unSale=="01"){
            m_btn1->setEnabled(true);
            m_btn1->setText("取消分摊(←)");
        }
        else{
            m_btn1->setEnabled(false);
            m_btn1->setText("已取消");
        }

        m_itemBtn1->setEnabled(false);        
        m_itemBtn2->setEnabled(false);
        m_itemBtn3->setEnabled(false);
        m_itemBtn4->setEnabled(false);
    }
    else if(m_type=="EDIT"){
        m_itemBtn1->setEnabled(false);
        m_itemBtn2->setEnabled(false);
        m_itemBtn3->setEnabled(false);
        m_itemBtn4->setEnabled(true);
        m_per->SetValue("");
        m_hitStaffName->setCurrentText("");
    }
}

//槽函数按键
void ErrorItemWidget::ButtonAddPress(){
    if(m_itemList->count()==100){
        m_parent->m_parent->MessageHit("已添满100个人");
        return;
    }
    //数值初始化
    double p=m_price->GetValue().toDouble();
    double sum=0;
    QString str1;
    QString str2;
    for(int i=0;i<m_itemList->count();i++){
        str1=m_itemList->item(i)->text().section('%',1,1);
        sum+=str1.remove(0,3).toDouble();
    }
    m_max=p-sum;
    //设置初始化
    connect(m_hitStaffName,&QComboBox::currentTextChanged,this,&ErrorItemWidget::HitBoxChanged);
    connect(m_per->m_edit,&QLineEdit::textEdited,this,&ErrorItemWidget::HitPriceChanged);

    m_per->SetDoubleNotype(2,m_max);
    //重置
    m_hitStaffName->clear();
    QStringList *strList;
    strList=m_parent->m_data->GetString("staffDatabase.db","IDname");
    m_staffCount=strList->count();
    if(strList!=NULL){
        bool isSame;
        for(int i=0;i<strList->count();i++){
            isSame=false;
            str1=strList->at(i);
            for(int i=0;i<m_itemList->count();i++){
                str2=m_itemList->item(i)->text().section("  ",0,0);
                if(str1==str2){
                    isSame=true;
                    break;
                }
                if(isSame)
                    break;
            }
            if(!isSame)
                m_hitStaffName->addItem(str1);
        }
        delete strList;
    }
    m_hitStaffName->setCurrentText("");
    m_hitStaffName->setFocus();
    //选择添加
    if(m_hitWidget->exec()){
        m_itemList->insertItem(m_itemList->count(),m_hitStaffName->currentText()
                               +"  "+QString::number(m_per->GetValue().toDouble()/m_price->GetValue().toDouble()*100,'f',1)
                               +"%  ¥"+m_per->GetValue());
        m_itemBtn4->setEnabled(true);
        //是否能再添加
        if(p<sum+m_per->GetValue().toDouble()){
            m_itemBtn1->setEnabled(false);
        }
        else if(p==sum+m_per->GetValue().toDouble())
            m_itemBtn1->setEnabled(false);
        else
            m_itemBtn1->setEnabled(true);
    }
    //取消链接
    disconnect(m_hitStaffName,&QComboBox::currentTextChanged,this,&ErrorItemWidget::HitBoxChanged);
    disconnect(m_per->m_edit,&QLineEdit::textEdited,this,&ErrorItemWidget::HitPriceChanged);
}
void ErrorItemWidget::HitBoxChanged(const QString& text){
    if(m_hitStaffName->findText(text)==-1||m_per->GetValue().toDouble()>m_max||m_per->GetValue().toDouble()==0
            ||m_staffCount==m_itemList->count())
        m_hitBtn1->setEnabled(false);
    else
        m_hitBtn1->setEnabled(true);
}
void ErrorItemWidget::HitPriceChanged(const QString& text){
    if(m_hitStaffName->findText(m_hitStaffName->currentText())==-1||text.toDouble()>m_max||text.toDouble()==0)
        m_hitBtn1->setEnabled(false);
    else
        m_hitBtn1->setEnabled(true);
}

void ErrorItemWidget::ButtonEditPress(){
    //数值初始化
    double p=m_price->GetValue().toDouble();
    double sum=0;
    QString str1;
    QString str2;
    int index=m_itemList->currentRow();
    QString text=m_itemList->currentItem()->text();
    text=text.section("  ",0,0);
    for(int i=0;i<m_itemList->count();i++){
        str1=m_itemList->item(i)->text().section('%',1,1);
        sum+=str1.remove(0,3).toDouble();
    }
    str1=m_itemList->item(index)->text().section('%',1,1);
    sum-=str1.remove(0,3).toDouble();
    m_max=p-sum;
    //设置初始化
    connect(m_hitStaffName,&QComboBox::currentTextChanged,this,&ErrorItemWidget::HitBoxChanged);
    connect(m_per->m_edit,&QLineEdit::textEdited,this,&ErrorItemWidget::HitPriceChanged);

    m_per->SetDoubleNotype(2,m_max);
    //重置
    m_hitStaffName->clear();
    QStringList *strList;
    strList=m_parent->m_data->GetString("staffDatabase.db","IDname");
    m_staffCount=strList->count();
    if(strList!=NULL){
        bool isSame;
        for(int i=0;i<strList->count();i++){
            isSame=false;
            str1=strList->at(i);
            if(text!=str1){
                for(int i=0;i<m_itemList->count();i++){
                    str2=m_itemList->item(i)->text().section("  ",0,0);
                    if(str1==str2){
                        isSame=true;
                        break;
                    }
                    if(isSame)
                        break;
                }
            }
            if(!isSame)
                m_hitStaffName->addItem(str1);
        }
        delete strList;
    }
    str1=text;
    str1=str1.section("  ",0,0);
    m_hitStaffName->setCurrentText(str1);

    str1=m_itemList->item(index)->text().section('%',1,1);
    m_per->SetValue(str1.remove(0,3));
    m_per->m_edit->setFocus();
    //是否修改
    if(m_hitWidget->exec()){
        //修改显示
        m_itemList->currentItem()->setText(m_hitStaffName->currentText()
                               +"  "+QString::number(m_per->GetValue().toDouble()/m_price->GetValue().toDouble()*100,'f',1)
                               +"%  ¥"+m_per->GetValue());
        m_itemBtn4->setEnabled(true);
        //是否能再添加
        if(p<sum+m_per->GetValue().toDouble()){
            m_itemBtn1->setEnabled(false);
        }
        else if(p==sum+m_per->GetValue().toDouble())
            m_itemBtn1->setEnabled(false);
        else
            m_itemBtn1->setEnabled(true);
    }
    //取消链接
    disconnect(m_hitStaffName,&QComboBox::currentTextChanged,this,&ErrorItemWidget::HitBoxChanged);
    disconnect(m_per->m_edit,&QLineEdit::textEdited,this,&ErrorItemWidget::HitPriceChanged);

}

void ErrorItemWidget::ButtonDelPress(){
    //删除显示
    QListWidgetItem *item = m_itemList->currentItem();
    m_itemList->removeItemWidget(item);
    delete item;
    if(m_itemList->count()==0){
        if(m_price->GetValue().toDouble()!=0) m_itemBtn1->setEnabled(true);
        else m_itemBtn1->setEnabled(false);
        m_itemBtn2->setEnabled(false);
        m_itemBtn3->setEnabled(false);
        m_itemBtn4->setEnabled(false);
    }
    else
        m_itemBtn1->setEnabled(true);
}

void ErrorItemWidget::ButtonClearPress(){
    //删除显示
    m_itemList->clear();
    if(m_price->GetValue().toDouble()!=0) m_itemBtn1->setEnabled(true);
    else m_itemBtn1->setEnabled(false);
    m_itemBtn2->setEnabled(false);
    m_itemBtn3->setEnabled(false);
    m_itemBtn4->setEnabled(false);
}

/*----键盘响应----*/
void ErrorItemWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && m_type!="DETAILS" && (key==Qt::Key_Enter || key==Qt::Key_Return))
        m_btn1->click();
}
