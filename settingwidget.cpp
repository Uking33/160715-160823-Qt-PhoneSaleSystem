//窗口
#include "settingwidget.h"
#include "mainwindow.h"
#include "iniData.h"
#include "printsum.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
//控件
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include "ubutton.h"
#include "uline.h"
//辅助
#include "type.h"
#include <QKeyEvent>

SettingWidget::SettingWidget(MainWindow *parent,IniData *data)
    :QDialog(parent)
{
    m_parent=parent;
    m_data=data;
    createWidget();
}

SettingWidget::~SettingWidget(){
    //---上边布局---
    delete m_upLayout;
    //---下边布局---
    delete m_btn1;
    delete m_btn2;
    delete m_downLayout;

    delete m_layout;
}
void SettingWidget::createWidget(){
    m_layout = new QVBoxLayout(this);
    //---上侧控件---
    m_upLayout = new QGridLayout();
    m_recordTitle=new ULine("单据标题:","");
    m_recordAddress=new ULine("公司地址:","");
    m_recordPhone=new ULine("联系方式:","");
    m_recordTitle->m_edit->setFixedWidth(400);
    m_recordAddress->m_edit->setFixedWidth(400);
    m_recordPhone->m_edit->setFixedWidth(400);
    m_recordAdditionLayout = new QHBoxLayout();
    m_recordAdditionLabel = new QLabel("单据备注:");
    m_recordAddition=new QTextEdit();
    m_recordAdditionLabel->setFixedWidth(60);
    m_recordAddition->setFixedSize(400,100);
    m_recordAdditionLayout->addWidget(m_recordAdditionLabel);
    m_recordAdditionLayout->addWidget(m_recordAddition);

    m_upLayout->addLayout(m_recordTitle,0,0);
    m_upLayout->addLayout(m_recordAddress,1,0);
    m_upLayout->addLayout(m_recordPhone,2,0);
    m_upLayout->addLayout(m_recordAdditionLayout,3,0);

    //---下侧控件---
    m_downLayout = new QHBoxLayout();
    m_btn1 = new UButton("保存(Ctrl+Enter)","",120,40,m_downLayout);
    m_btn2 = new UButton("预览(Enter)","",120,40,m_downLayout);
    m_btn3 = new UButton("返回(Esc)","Esc",120,40,m_downLayout);
    m_btn1->setFocusPolicy(Qt::NoFocus);
    m_btn2->setFocusPolicy(Qt::NoFocus);
    m_btn3->setFocusPolicy(Qt::NoFocus);
    m_btn1->setFont(QFont("Bold",10));
    m_btn2->setFont(QFont("Bold",10));
    m_btn3->setFont(QFont("Bold",10));
    connect(m_btn1,SIGNAL(clicked()),this,SLOT(Save()));
    connect(m_btn2,SIGNAL(clicked()),this,SLOT(Watch()));
    connect(m_btn3,&QPushButton::clicked,this,&SettingWidget::close);
    //窗口
    setFixedSize(500,300);
    setWindowTitle("设置");
    //---布局---
    m_layout->addLayout(m_downLayout,1);
    m_layout->addLayout(m_upLayout,4);
    setLayout(m_layout);
}
//获取重置
void SettingWidget::reWidget(){    
    m_recordTitle->SetValue(m_data->m_recordTitle);
    m_recordAddress->SetValue(m_data->m_recordAddress);
    m_recordPhone->SetValue(m_data->m_recordPhone);
    m_recordAddition->setText(m_data->m_recordAddition);
}
void SettingWidget::Save(){
    m_data->m_recordTitle=m_recordTitle->GetValue();
    m_data->m_recordAddress=m_recordAddress->GetValue();
    m_data->m_recordPhone=m_recordPhone->GetValue();
    m_data->m_recordAddition=m_recordAddition->toPlainText();
    m_data->recordSave();
    m_parent->MessageHit("已保存");
}

void SettingWidget::Watch(){//预览
    //初始化
    QString str1,str2,str3,str4;
    str1=m_data->m_recordTitle;
    str2=m_data->m_recordAddress;
    str3=m_data->m_recordPhone;
    str4=m_data->m_recordAddition;
    m_data->m_recordTitle=m_recordTitle->GetValue();
    m_data->m_recordAddress=m_recordAddress->GetValue();
    m_data->m_recordPhone=m_recordPhone->GetValue();
    m_data->m_recordAddition=m_recordAddition->toPlainText();
    PhoneRecordList *rec=new PhoneRecordList();
    //预览
    m_parent->m_printsum->Watch(rec);
    //恢复
    delete rec;
    m_data->m_recordTitle=str1;
    m_data->m_recordAddress=str2;
    m_data->m_recordPhone=str3;
    m_data->m_recordAddition=str4;
}
/*----键盘响应----*/
void SettingWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(key==Qt::Key_Enter || key==Qt::Key_Return){
        if (event->modifiers()==(Qt::ControlModifier))
            m_btn1->click();
        else
            m_btn2->click();
    }
}

