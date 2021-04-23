//窗口
#include "recorditemwidget.h"
#include "recordwidget.h"
//布局
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
//控件
#include <QLabel>
#include <QListWidget>
#include "ubutton.h"
#include "uline.h"
//数据
#include "type.h"
//辅助
#include <QKeyEvent>

RecordItemWidget::RecordItemWidget(RecordWidget *parent)
    :QDialog(parent)
{
    m_parent=parent;
    createWidget();
}

RecordItemWidget::~RecordItemWidget(){
    delete m_layout;
}
void RecordItemWidget::createWidget(){
    m_layout = new QVBoxLayout(this);
    //---上侧控件---
    m_upLayout = new QHBoxLayout();
    m_btn1 = new UButton("再次打印(Enter)","",120,30,m_upLayout);
    m_btn2 = new UButton("打印预览(Space)","Space",120,30,m_upLayout);
    m_btn3 = new UButton("退货(←)","Backspace",120,30,m_upLayout);
    m_btn4 = new UButton("返回(Esc)","Esc",120,30,m_upLayout);
    m_btn1->setFont(QFont("Bold",10));
    m_btn2->setFont(QFont("Bold",10));
    m_btn3->setFont(QFont("Bold",10));
    m_btn4->setFont(QFont("Bold",10));
    connect(m_btn1,SIGNAL(clicked()),m_parent,SLOT(ButtonDetailsPrint()));
    connect(m_btn2,SIGNAL(clicked()),m_parent,SLOT(ButtonDetailsWatch()));
    connect(m_btn3,SIGNAL(clicked()),m_parent,SLOT(ButtonDetailsUnSale()));
    connect(m_btn4,&QPushButton::clicked,this,&RecordItemWidget::close);
    //---下侧控件---
    m_downLayout = new QGridLayout();
    m_ID=new ULine("单号:","");//单号
    m_outTime=new ULine("出单时间:","");//出单时间
    m_shopAssistant=new ULine("营业员:","");//营业员
    m_cashier=new ULine("收银员:","");//收银员
    m_salePrice=new ULine("售价(¥):","");//售价
    m_gather=new ULine("毛利(¥):","");//毛利

    //项目
    m_itemsLayout = new QHBoxLayout();
    m_itemsLabel = new QLabel("账单:");
    m_itemsList = new QListWidget();
    m_itemsLabel->setFixedSize(60,25);
    m_itemsList->setFixedSize(200,55);
    m_itemsLayout->addWidget(m_itemsLabel);
    m_itemsLayout->addWidget(m_itemsList);
    m_itemsLayout->setAlignment(Qt::AlignLeft);
    connect(m_itemsList,&QListWidget::itemDoubleClicked,[=](QListWidgetItem * item){
        QString text=item->text();
        if(text.contains(','))
            m_parent->ButtonDetailsItem("phone",text.right(15));
        else
            m_parent->ButtonDetailsItem("tool",m_record->dataList->at(m_itemsList->currentRow()));
    });
    m_downLayout->addLayout(m_ID,0,0);
    m_downLayout->addLayout(m_outTime,1,0);
    m_downLayout->addLayout(m_shopAssistant,2,0);
    m_downLayout->addLayout(m_cashier,3,0);
    m_downLayout->addLayout(m_salePrice,0,1);
    m_downLayout->addLayout(m_gather,1,1);
    m_downLayout->addLayout(m_itemsLayout,2,1,2,1);

    //查看
    m_ID->SetReadOnly(true);
    m_outTime->SetReadOnly(true);
    m_shopAssistant->SetReadOnly(true);
    m_cashier->SetReadOnly(true);
    m_salePrice->SetReadOnly(true);
    m_gather->SetReadOnly(true);
    //窗口
    setFixedSize(600,200);
    setWindowTitle("查看销售记录");
    //---布局---
    m_layout->addLayout(m_upLayout,1);
    m_layout->addLayout(m_downLayout,3);
    setLayout(m_layout);
}
//获取重置
void RecordItemWidget::reWidget(PhoneRecordList * data){
    m_record=data;
    m_ID->SetValue(data->GetData("show","ID"));//单号
    m_outTime->SetValue(data->GetData("show","outTime"));//出单时间
    m_shopAssistant->SetValue(data->GetData("show","shopAssistant"));//营业员
    m_cashier->SetValue(data->GetData("show","cashier"));//收银员
    m_salePrice->SetValue(QString::number(data->price,'f',2));//售价
    m_gather->SetValue(QString::number(data->gather,'f',2));//毛利
    m_itemsList->clear();
    foreach(PhoneRecord* data,(*data->dataList)){
        if(data->IMEI!=0)
            m_itemsList->addItem(data->phoneModels+","+data->IMEI);
        else
            m_itemsList->addItem(data->phoneModels);
    }
    //是否已退货
    if(m_record->unSale.length()==2 && m_record->unSale.left(1)=="0"){
        m_btn1->setEnabled(true);
        m_btn3->setEnabled(true);
        m_btn1->setText("再次打印(Enter)");
        m_btn3->setText("退货(←)");
    }
    else{
        m_btn1->setEnabled(false);
        m_btn3->setEnabled(false);
        m_btn1->setText("已退货");
        m_btn3->setText("已退货");
    }
}
/*----键盘响应----*/
void RecordItemWidget::keyPressEvent(QKeyEvent *event){
    int key=event->key();
    if(this->isVisible() && (key==Qt::Key_Enter || key==Qt::Key_Return))
        m_btn1->click();
}
