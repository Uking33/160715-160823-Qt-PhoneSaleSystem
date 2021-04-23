//窗口
#include "loginwidget.h"
#include "mainwindow.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
//控件
#include "ubutton.h"
#include "uline.h"
//数据
#include "data.h"
#include "iniData.h"
//辅助
#include <QKeyEvent>
#include <QMessageBox>
#include <QLineEdit>

LogInWidget::LogInWidget(Data *data, IniData *ini, short type, QString ID, QString lastPassword){
    m_type=type;
    m_data=data;
    m_ini=ini;
    m_lastPassword=lastPassword;
    createWidget(ID);
}

LogInWidget::~LogInWidget(){
}
void LogInWidget::createWidget(QString ID){
    m_layout = new QVBoxLayout(this);
    //---上侧控件---
    m_upLayout = new QGridLayout();
    m_ID=new ULine("工号:","四位数字");
    m_ID->SetIntNotype(9999);
    m_ID->m_edit->setContextMenuPolicy(Qt::NoContextMenu);
    m_ID->SetValue(ID);
    m_password=new ULine("密码:","4-15位数字");
    m_password->m_edit->setEchoMode(QLineEdit::Password);
    m_password->m_edit->setMaxLength(15);
    m_password->m_edit->setContextMenuPolicy(Qt::NoContextMenu);
    m_upLayout->addLayout(m_ID,0,0);
    m_upLayout->addLayout(m_password,1,0);
    if(m_type==0)
        m_ID->SetValue(m_ini->m_loginName);
    else if(m_type==1)
        m_password->SetLabel("原始密码");
    else if(m_type==2)
        m_password->SetLabel("新密码");
    else if(m_type==3)
        m_password->SetLabel("再次新密码");
    else {
        m_ID->m_edit->setReadOnly(true);
        m_ID->m_edit->setFocusPolicy(Qt::NoFocus);
    }
    //---下侧控件---
    m_downLayout = new QHBoxLayout();
    if(m_type==0)
        m_btn1 = new UButton("登录(Enter)","",80,30,m_downLayout);
    else
        m_btn1 = new UButton("确定(Enter)","",80,30,m_downLayout);
    m_btn2 = new UButton("退出(Esc)","Esc",80,30,m_downLayout);
    m_btn1->setFocusPolicy(Qt::NoFocus);
    m_btn2->setFocusPolicy(Qt::NoFocus);
    m_btn1->setFont(QFont("Bold",9));
    m_btn2->setFont(QFont("Bold",9));
    connect(m_btn1,SIGNAL(clicked()),this,SLOT(Login()));
    connect(m_btn2,&QPushButton::clicked,this,&LogInWidget::reject);
    //窗口
    setFixedSize(300,150);
    if(m_type==0)
        setWindowTitle("登录");
    else if(m_type>=1 && m_type<=3)
        setWindowTitle("修改密码");
    else if(m_type==4)
        setWindowTitle("验证");
    //---布局---
    m_layout->addLayout(m_upLayout);
    m_layout->addLayout(m_downLayout);
    setLayout(m_layout);
    //焦点
    if(m_ID->GetValue()==""){
        m_ID->SetFocus();
    }
    else{
        m_password->SetFocus();
    }

}

void LogInWidget::Login(){//登录
    if(m_ID->GetValue().length()!=4){
        QMessageBox message(QMessageBox::Information, "提示", "工号必须4位数");
        message.exec();
        return;
    }
    if(m_password->GetValue().length()<4){
        QMessageBox message(QMessageBox::Information, "提示", "密码必须4-15位数");
        message.exec();
        return;
    }
    if(m_type==2){
        if(m_lastPassword!=m_password->GetValue())
            accept();
        else{
            QMessageBox message(QMessageBox::Information, "提示", "新密码和原始密码一样");
            message.exec();
        }
        return;
    }
    if(m_type==3){
        if(m_lastPassword==m_password->GetValue())
            accept();
        else{
            QMessageBox message(QMessageBox::Information, "提示", "两次密码不一致");
            message.exec();
        }
        return;
    }
    if(m_type==4){//验证
        if(m_data->Login(m_ID->GetValue(),m_password->GetValue()))
            accept();
        else{
            QMessageBox message(QMessageBox::Information, "提示", "密码不正确");
            message.exec();
        }
        return;
    }
    QString text;
    short re=m_data->Login(m_ID->GetValue(),m_password->GetValue());
    switch(re){
        case -2:
            text="系统错误，请联系开发人员";
            break;
        case -1:
            text="该工号未注册";
            m_ID->m_edit->setFocus();
            m_ID->m_edit->selectAll();
            break;
        case 0:
            text="密码错误";
            m_password->m_edit->setFocus();
            m_password->m_edit->selectAll();
            break;
        case 1:
            accept();
            break;
    }
    if(re!=1){
        QMessageBox message(QMessageBox::Information, "提示", text);
        message.exec();
    }
    if(re==1 && m_type==0){
        m_ini->m_loginName=m_ID->GetValue();
        m_ini->loginSave();
    }
}
QString LogInWidget::GetID(){
    return m_ID->GetValue();
}
QString LogInWidget::GetName(){
    QString re;
    re=m_data->GetStaffNameBasedata(m_ID->GetValue());
    return re;
}
QString LogInWidget::GetPassword(){
    return m_password->GetValue();
}

/*----键盘响应----*/
void LogInWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return))
        m_btn1->click();
}
