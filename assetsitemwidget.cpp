//窗口
#include "assetsitemwidget.h"
#include "assetswidget.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
//控件
#include "ubutton.h"
#include "uline.h"
//数据
#include "type.h"
//辅助
#include <QKeyEvent>

AssetsItemWidget::AssetsItemWidget(AssetsWidget *parent,QString type)
    :QDialog(parent)
{
    m_parent=parent;
    m_type=type;
    createWidget();
}

AssetsItemWidget::~AssetsItemWidget(){
    delete m_layout;
}
void AssetsItemWidget::createWidget(){
    m_layout = new QVBoxLayout(this);
    //---下侧控件---
    m_downLayout = new QVBoxLayout;
    m_name=new ULine("资产名称:","");
    m_count=new ULine("数量(个):","");
    m_price=new ULine("单价(￥):","系统保留小数点后2位");
    m_addition=new ULine("备注:","");

    m_name->SetMaxLength(50);
    m_count->SetInt(999999,1);
    m_price->SetDouble(2,999999);
    m_addition->SetMaxLength(1000);

    m_downLayout->addLayout(m_name);
    m_downLayout->addLayout(m_count);
    m_downLayout->addLayout(m_price);
    m_downLayout->addLayout(m_addition);

    //---上侧控件---
    m_upLayout = new QHBoxLayout;
    m_btn1 = new UButton("确定(Enter)","",120,30,m_upLayout);
    m_btn2 = new UButton("返回(Esc)","Esc",120,30,m_upLayout);
    m_btn1->setFocusPolicy(Qt::NoFocus);
    m_btn2->setFocusPolicy(Qt::NoFocus);
    m_btn1->setFont(QFont("Bold",10));
    m_btn2->setFont(QFont("Bold",10));
    if(m_type=="ADD"){
        connect(m_btn1,&UButton::clicked,m_parent,&AssetsWidget::ButtonAddPressEnter);
        connect(m_btn2,&UButton::clicked,this,&AssetsItemWidget::close);
    }
    else if(m_type=="DETAILS"){
        connect(m_btn2,&QPushButton::clicked,this,&AssetsItemWidget::close);
    }
    else if(m_type=="EDIT"){
        connect(m_btn1,&QPushButton::clicked,m_parent,&AssetsWidget::ButtonEditPressEnter);
        connect(m_btn2,&QPushButton::clicked,this,&AssetsItemWidget::close);
    }
    //查看
    if(m_type=="DETAILS"){
        m_btn1->setEnabled(false);
        m_name->SetReadOnly(true);
        m_count->SetReadOnly(true);
        m_price->SetReadOnly(true);
        m_addition->SetReadOnly(true);
    }
    //窗口
    setFixedSize(300,200);
    if(m_type=="ADD")
        setWindowTitle("添加固定资产");
    else if(m_type=="DETAILS")
        setWindowTitle("查看固定资产");
    else if(m_type=="EDIT")
        setWindowTitle("修改固定资产");
    //---布局---
    m_layout->addLayout(m_upLayout);
    m_layout->addLayout(m_downLayout);
    setLayout(m_layout);
}
//空白重置
void AssetsItemWidget::reWidget(){
    m_name->SetValue("");
    m_count->SetValue("");
    m_price->SetValue("");
    m_addition->SetValue("");
}
//获取重置
void AssetsItemWidget::reWidget(AssetsData * data){
    m_record=data;
    m_name->SetValue(data->name);
    m_count->SetValue(QString::number(data->count));
    m_price->SetValue(QString::number(data->price));
    m_addition->SetValue(data->addition);
}

/*----键盘响应----*/
void AssetsItemWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return))
        m_btn1->click();
}
