//窗口
#include "toolitemwidget.h"
#include "ToolWidget.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
//控件
#include "ubutton.h"
#include "uline.h"
#include "udatebox.h"
//数据
#include "type.h"
//辅助
#include <QKeyEvent>

ToolItemWidget::ToolItemWidget(ToolWidget *parent,QString type)
    :QDialog(parent)
{
    m_parent=parent;
    m_type=type;
    createWidget();
}

ToolItemWidget::~ToolItemWidget(){
    delete m_layout;
}

void ToolItemWidget::createWidget(){
    m_layout = new QVBoxLayout(this);
    //---上侧控件---
    m_upLayout = new QHBoxLayout();
    //创建
    if(m_type!="DETAILS"){
        m_btn1 = new UButton("确定(Enter)","",100,30,m_upLayout);
        m_btn1->setFont(QFont("Bold",10));
        m_btn2 = new UButton("返回(Esc)","Esc",100,30,m_upLayout);
        m_btn2->setFont(QFont("Bold",10));
        if(m_type=="ADD")
            connect(m_btn1,&QPushButton::clicked,m_parent,&ToolWidget::ButtonAddPressEnter);
        else if(m_type=="EDIT")
            connect(m_btn1,&QPushButton::clicked,m_parent,&ToolWidget::ButtonEditPressEnter);
        connect(m_btn2,&QPushButton::clicked,this,&ToolItemWidget::close);
    }
    else{
        m_btn1 = new UButton("添加(Enter)","",85,30,m_upLayout);
        m_btn2 = new UButton("添加并结帐","",85,30,m_upLayout);
        m_btn3 = new UButton("返回(Esc)","Esc",85,30,m_upLayout);
        m_btn1->setFont(QFont("Bold",10));
        m_btn2->setFont(QFont("Bold",10));
        m_btn3->setFont(QFont("Bold",10));
        connect(m_btn1,&UButton::clicked,m_parent,&ToolWidget::ButtonDetailsSaleAddPress);
        connect(m_btn2,&UButton::clicked,m_parent,&ToolWidget::ButtonDetailsSaleSumPress);
        connect(m_btn3,&UButton::clicked,this,&ToolItemWidget::close);
    }
    //窗口
    if(m_type=="ADD"){
        setWindowTitle("添加新配件");
    }
    else if(m_type=="EDIT"){
        setWindowTitle("修改配件信息");
    }
    else if(m_type=="DETAILS"){
        setWindowTitle("查看配件信息");
    }
    setFixedSize(300,200);
    //---下侧控件---
    m_downLayout = new QVBoxLayout();
    //创建
    m_name=new ULine("配件名:","请输入配件名称");
    m_name->SetMaxLength(30);
    m_count=new ULine("数量:","请输入配件数量");
    m_count->SetInt(999999);
    if(m_parent->m_model){
        m_buyingPrice=new ULine("成本价(¥):","系统保留小数点后2位");
        m_buyingPrice->SetDouble(2,999999);
    }
    m_salingPrice=new ULine("零售价(¥):","系统保留小数点后2位" );
    m_salingPrice->SetDouble(2,999999);
    m_unit=new ULine("单位:","请输入配件单位");
    m_unit->SetMaxLength(10);
    //布局
    m_downLayout->addLayout(m_name);
    m_downLayout->addLayout(m_count);
    if(m_parent->m_model) m_downLayout->addLayout(m_buyingPrice);
    m_downLayout->addLayout(m_salingPrice);
    m_downLayout->addLayout(m_unit);
    //连接按钮
    if(m_type!="DETAILS"){
        connect(m_name,&ULine::textEdited,this,&ToolItemWidget::TextEdit);
        connect(m_count,&ULine::textEdited,this,&ToolItemWidget::TextEdit);
        connect(m_unit,&ULine::textEdited,this,&ToolItemWidget::TextEdit);
    }
    //特殊处理
    if(m_type=="DETAILS"){
        m_name->SetReadOnly(true);
        m_count->SetReadOnly(true);
        if(m_parent->m_model)
            m_buyingPrice->SetReadOnly(true);
        m_salingPrice->SetReadOnly(true);
        m_unit->SetReadOnly(true);
    }
    //---布局---
    m_layout->addLayout(m_upLayout);
    m_layout->addLayout(m_downLayout);
    setLayout(m_layout);
}

void ToolItemWidget::TextEdit(){
    if(m_name->GetValue()==""||(m_name->GetValue().toLongLong()!=0&&m_name->GetValue().length()==15)||m_count->GetValue().toFloat()==0||m_unit->GetValue()=="")
        m_btn1->setEnabled(false);
    else
        m_btn1->setEnabled(true);
}

//空白重置
void ToolItemWidget::reWidget(){
    //重置
    m_name->ReValue();
    m_count->SetValue(1);
    m_buyingPrice->ReValue();
    m_salingPrice->ReValue();
    m_unit->ReValue();
    m_btn1->setEnabled(false);
}

//获取重置
void ToolItemWidget::reWidget(ToolData *data){
    m_tool=data;
    //重置
    m_name->SetValue(data->name);
    m_count->SetValue(data->count);
    if(m_parent->m_model) m_buyingPrice->SetValue(data->buyingPrice);
    m_salingPrice->SetValue(data->salingPrice);
    m_unit->SetValue(data->unit);
    if(m_type=="DETAILS"){
        m_btn1->setEnabled(true);
        m_btn2->setEnabled(true);
        m_btn3->setEnabled(true);
        m_count->SetLabel("剩余库存");
    }
    else{
        m_name->SetReadOnly(false);        
        if(m_parent->m_model) m_buyingPrice->SetReadOnly(false);
        m_unit->SetReadOnly(false);
        m_count->SetIntNotype(999999);
        m_btn1->setEnabled(true);
    }
}

/*----键盘响应----*/
void ToolItemWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return)){
        if (m_type=="DETAILS" && (event->modifiers()==(Qt::ControlModifier)))
            m_btn2->click();
        else
            m_btn1->click();
    }
}
