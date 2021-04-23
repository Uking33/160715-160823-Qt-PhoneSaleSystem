//窗口
#include "staffitemwidget.h"
#include "staffwidget.h"
#include "mainwindow.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
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
#include <QIntValidator>
#include <QComboBox>
#include <QKeyEvent>

StaffItemWidget::StaffItemWidget(StaffWidget *parent,QString type)
    :QDialog(parent)
{
    m_parent=parent;
    m_type=type;
    createWidget();
}

StaffItemWidget::~StaffItemWidget(){
    delete m_layout;
}
void StaffItemWidget::createWidget(){
    m_layout = new QVBoxLayout(this);
    //---下侧控件---
    m_downLayout = new QGridLayout();
    m_ID=new ULine("工号:","四位数字");
    m_ID->SetInt(9999);
    m_name=new ULine("姓名:","请输入员工的姓名");
    m_name->SetMaxLength(30);
    m_inTime=new QHBoxLayout();
    m_inTimeLabel=new QLabel("入职:");
    if(m_type=="ADD")
        m_inTimeBox = new UDateBox(1);
    if(m_type=="EDIT")
        m_inTimeBox = new UDateBox(0);
    if(m_type=="DETAILS"){
        m_inTimeBox = new UDateBox(0);
        QLineEdit *t_Line;
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_inTimeBox->m_comboBoxYear->setLineEdit(t_Line);
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_inTimeBox->m_comboBoxMonth->setLineEdit(t_Line);
        t_Line=new QLineEdit();
        t_Line->setReadOnly(true);
        m_inTimeBox->m_comboBoxDay->setLineEdit(t_Line);
    }
    m_inTimeLabel->setFixedSize(60,25);
    m_inTimeBox->setFixedSize(200,25);
    m_inTime->addWidget(m_inTimeLabel);
    m_inTime->addWidget(m_inTimeBox);
    m_addition=new ULine("备注:","");
    m_addition->SetMaxLength(1000);

    m_downLayout->addLayout(m_ID,0,0);
    m_downLayout->addLayout(m_name,1,0);
    m_downLayout->addLayout(m_inTime,2,0);
    m_downLayout->addLayout(m_addition,3,0);
    if(m_type=="DETAILS"){
        m_error=new ULine("分摊费用￥:","");
        m_error->SetReadOnly(true);
        m_downLayout->addLayout(m_error,4,0);
    }
    if(m_type=="DETAILS"){
        m_ID->m_edit->setReadOnly(true);
        m_name->m_edit->setReadOnly(true);
        m_addition->m_edit->setReadOnly(true);

        m_findTimeS=new QHBoxLayout();
        m_findTimeSLabel=new QLabel("业绩起始:");
        m_findTimeSBox = new UDateBox(2);
        m_findTimeSLabel->setFixedSize(60,25);
        m_findTimeSBox->setFixedSize(200,25);
        m_findTimeS->addWidget(m_findTimeSLabel);
        m_findTimeS->addWidget(m_findTimeSBox);

        m_findTimeE=new QHBoxLayout();
        m_findTimeELabel=new QLabel("业绩结束:");
        m_findTimeEBox = new UDateBox(3,m_findTimeSBox);
        m_findTimeELabel->setFixedSize(60,25);
        m_findTimeEBox->setFixedSize(200,25);
        m_findTimeE->addWidget(m_findTimeELabel);
        m_findTimeE->addWidget(m_findTimeEBox);

        m_count=new ULine("销售量:","");
        m_price=new ULine("销售额￥:","");
        m_gather=new ULine("毛利￥:","");
        m_count->m_edit->setReadOnly(true);
        m_price->m_edit->setReadOnly(true);
        m_gather->m_edit->setReadOnly(true);

        m_downLayout->addLayout(m_findTimeS,0,1);
        m_downLayout->addLayout(m_findTimeE,1,1);
        m_downLayout->addLayout(m_count,2,1);
        m_downLayout->addLayout(m_price,3,1);
        m_downLayout->addLayout(m_gather,4,1);

        //业绩
        m_itemsLayout = new QHBoxLayout();
        m_itemsLabel = new QLabel("业绩:");
        m_itemsList = new QListWidget();
        m_itemsLabel->setFixedSize(60,25);
        m_itemsList->setFixedSize(485,200);
        m_itemsLayout->addWidget(m_itemsLabel);
        m_itemsLayout->addWidget(m_itemsList);
        m_itemsLayout->setAlignment(Qt::AlignLeft);
        connect(m_itemsList,&QListWidget::itemDoubleClicked,[=](QListWidgetItem * item){
            m_parent->m_parent->ItemDetails("record",item->text().mid(8,12));
        });

        //分摊
        m_errorsLayout = new QHBoxLayout();
        m_errorsLabel = new QLabel("分摊:");
        m_errorsList = new QListWidget();
        m_errorsLabel->setFixedSize(60,25);
        m_errorsList->setFixedSize(485,200);
        m_errorsLayout->addWidget(m_errorsLabel);
        m_errorsLayout->addWidget(m_errorsList);
        m_errorsLayout->setAlignment(Qt::AlignLeft);
        connect(m_errorsList,&QListWidget::itemDoubleClicked,[=](QListWidgetItem * item){
            QString str=item->text(); str=str.section(" ",1,1); str=str.remove(0,3);
            foreach(StaffError* dd,(*m_record->errorList))
                if(dd->name==str){
                    m_parent->m_parent->ItemDetails("error",dd->ID);
                    break;
                }
        });

        m_downLayout->addLayout(m_itemsLayout,5,0,2,2);
        m_downLayout->addLayout(m_errorsLayout,7,0,2,2);
    }

    //---上侧控件---
    m_upLayout = new QHBoxLayout();
    m_btn1 = new UButton("确定(Enter)","",100,30,m_upLayout);
    if(m_type=="DETAILS")
        m_btn1->setText("查询(Enter)");
    m_btn2 = new UButton("返回(Esc)","Esc",100,30,m_upLayout);
    m_btn1->setFocusPolicy(Qt::NoFocus);
    m_btn2->setFocusPolicy(Qt::NoFocus);
    m_btn1->setFont(QFont("Bold",10));
    m_btn2->setFont(QFont("Bold",10));
    connect(m_btn2,&QPushButton::clicked,this,&StaffItemWidget::close);
    if(m_type=="ADD")
        connect(m_btn1,&QPushButton::clicked,m_parent,&StaffWidget::ButtonAddPressEnter);
    else if(m_type=="EDIT")
        connect(m_btn1,&QPushButton::clicked,m_parent,&StaffWidget::ButtonEditPressEnter);
    else if(m_type=="DETAILS")
        connect(m_btn1,&QPushButton::clicked,this,&StaffItemWidget::ButtonFindPress);
    //窗口
    if(m_type=="ADD"){
        setWindowTitle("添加新员工");
        setFixedSize(300,200);
    }
    else if(m_type=="EDIT"){
        setWindowTitle("修改员工信息");
        setFixedSize(300,200);
    }
    else if(m_type=="DETAILS"){
        setWindowTitle("查看员工信息");
        setFixedSize(600,650);
    }
    //---布局---
    m_layout->addLayout(m_upLayout,1);
    m_layout->addLayout(m_downLayout,3);
    setLayout(m_layout);
}
//空白重置
void StaffItemWidget::reWidget(){
    //重置
    QString str;
    str=m_parent->m_data->GetNextID("staffDatabase.db","ID");
    if(str.length()==4){
        m_ID->SetValue(str);
        m_name->m_edit->setFocus();
    }
    else if(str=="RangeUp"){
        m_parent->m_parent->MessageHit("最大工号已达9999，请手动输入工号");
        m_ID->SetValue("");
        m_ID->m_edit->setFocus();
    }
    else{
        m_parent->m_parent->MessageHit("未知错误，请手动输入工号");
        m_ID->SetValue("");
        m_ID->m_edit->setFocus();
    }
    m_name->SetValue("");
    m_inTimeBox->m_state=1;
    m_inTimeBox->Update();
    m_addition->SetValue("");
}
//获取重置
void StaffItemWidget::reWidget(StaffDataList *data){
    m_record=data;
    m_ID->SetValue(data->GetData("show","ID"));
    m_name->SetValue(data->GetData("show","name"));
    m_inTimeBox->Update(data->inDate);
    m_addition->SetValue(data->GetData("show","addition"));

    if(m_type=="DETAILS"){
        //入职时间清除处理
        QString str;
        m_inTimeBox->DisconnectBox();
        str=m_inTimeBox->m_comboBoxYear->currentText();
        m_inTimeBox->m_comboBoxYear->clear();
        m_inTimeBox->m_comboBoxYear->setCurrentText(str);
        str=m_inTimeBox->m_comboBoxMonth->currentText();
        m_inTimeBox->m_comboBoxMonth->clear();
        m_inTimeBox->m_comboBoxMonth->setCurrentText(str);
        str=m_inTimeBox->m_comboBoxDay->currentText();
        m_inTimeBox->m_comboBoxDay->clear();
        m_inTimeBox->m_comboBoxDay->setCurrentText(str);
        m_inTimeBox->ConnectBox();
        //查询时间
        m_findTimeSBox->Update();
        QString time=m_parent->m_outdateSBox->GetData();
        m_findTimeSBox->DisconnectBox();
        if(time.length()>=4){
            str=time.left(4);
            m_findTimeSBox->ChooseYear(str);
            m_findTimeSBox->m_comboBoxYear->setCurrentText(str);
        }
        if(time.length()>=6){
            str=time.mid(4,2);
            if(str.at(0)=='0')
                str.remove(0,1);
            m_findTimeSBox->ChooseMonth(str);
            m_findTimeSBox->m_comboBoxMonth->setCurrentText(str);
        }
        if(time.length()==8){
            str=time.mid(6,2);
            if(str.at(0)=='0')
                str.remove(0,1);
            m_findTimeSBox->ChooseDay(str);
            m_findTimeSBox->ConnectBox();
            m_findTimeSBox->m_comboBoxDay->setCurrentText(str);
        }

        time=m_parent->m_outdateEBox->GetData();
        m_findTimeEBox->DisconnectBox();
        if(time.length()>=4){
            str=time.left(4);
            m_findTimeEBox->ChooseYear(str);
            m_findTimeEBox->m_comboBoxYear->setCurrentText(str);
        }
        if(time.length()>=6){
            str=time.mid(4,2);
            if(str.at(0)=='0')
                str.remove(0,1);
            m_findTimeEBox->ChooseMonth(str);
            m_findTimeEBox->m_comboBoxMonth->setCurrentText(str);
        }
        if(time.length()==8){
            str=time.mid(6,2);
            if(str.at(0)=='0')
                str.remove(0,1);
            m_findTimeEBox->ChooseDay(str);
            m_findTimeEBox->m_comboBoxDay->setCurrentText(str);
        }
        m_findTimeEBox->ConnectBox();
        ButtonFindPress();
    }
}

void StaffItemWidget::ButtonFindPress(){
    QString time1=m_findTimeSBox->GetData();
    QString time2=m_findTimeEBox->GetData();
    QString str;
    //业绩
    m_itemsList->clear();
    foreach(StaffRec* dd,*(m_record->recList)){
        if(dd->date>=time1 && dd->date<=time2 && dd->work.contains("营业员")){
            if(dd->unSale.length()==16){
                str="结后退货 ";
                str+="单号:"+dd->ID+" 职务:"+dd->work+
                                     " 销售额(¥):"+QString::number(dd->price,'d',2)+
                                     " 毛利(¥):"+QString::number(dd->gather,'d',2);
            }
            else if(dd->unSale.left(1)=="0"){
                str="已经出售 ";
                str+="单号:"+dd->ID+" 职务:"+dd->work+
                                     " 销售额(¥):"+QString::number(dd->price,'d',2)+
                                     " 毛利(¥):"+QString::number(dd->gather,'d',2);
            }
            else{
                str="已经退货 ";
                str+="单号:"+dd->ID+" 职务:"+dd->work+
                                     " 销售额(¥):"+QString::number(0,'d',2)+
                                     " 毛利(¥):"+QString::number(0,'d',2);
            }
            m_itemsList->addItem(str);
        }
        if(dd->unSale.length()==16 &&
                (dd->unSale.left(8)>=time1 && dd->unSale.left(8)<=time2)){
            str="退货扣减 ";
            str+="单号:"+dd->ID+" 职务:"+dd->work+
                                 " 销售额(¥):"+QString::number(-dd->price,'d',2)+
                                 " 毛利(¥):"+QString::number(-dd->gather,'d',2);
            m_itemsList->addItem(str);
        }
    }
    m_count->SetValue(QString("%1(营业:%2,收银:%3)").arg(m_record->GetData("show","count",time1,time2)).arg(m_record->GetData("show","shopAssistCount",time1,time2)).arg(m_record->GetData("show","cashyCount",time1,time2)));
    m_price->SetValue(m_record->GetData("show","shopAssistPrice",time1,time2).remove(0,1));
    m_gather->SetValue(m_record->GetData("show","gather",time1,time2).remove(0,1));
    //分摊
    double money=0;
    m_errorsList->clear();
    foreach(StaffError* dd,*(m_record->errorList)){        
        if(dd->date>=time1 && dd->date<=time2){
            if(dd->unSale.length()==16){
                str="结后取消 ";
                str+="名称:"+dd->name+" 时间:"+dd->date+
                 " 金额(¥):"+QString::number(dd->price,'d',2)+" 原因:";
                money+=dd->price;
            }
            else if(dd->unSale.left(1)=="0"){
                str="已经分摊 ";
                str+="名称:"+dd->name+" 时间:"+dd->date+
                 " 金额(¥):"+QString::number(dd->price,'d',2)+" 原因:";
                money+=dd->price;
            }
            else{
                str="已经取消 ";
                str+="名称:"+dd->name+" 时间:"+dd->date+
                 " 金额(¥):"+QString::number(0,'d',2)+" 原因:";
            }
            if(dd->reason=="")
                str+="无";
            else
                str+=dd->reason;
            m_errorsList->addItem(str);
        }
        if(dd->unSale.length()==16 &&
                (dd->unSale.left(8)>=time1 && dd->unSale.left(8)<=time2)){
            str="取消返还 ";
            str+="名称:"+dd->name+" 时间:"+dd->date+
             " 金额(¥):"+QString::number(-dd->price,'d',2)+" 原因:";
            if(dd->reason=="")
                str+="无";
            else
                str+=dd->reason;
            m_errorsList->addItem(str);
            money-=dd->price;
        }
    }
    m_error->SetValue(QString::number(money,'d',2));
}

/*----键盘响应----*/
void StaffItemWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return)){
        m_btn1->click();
    }
}
