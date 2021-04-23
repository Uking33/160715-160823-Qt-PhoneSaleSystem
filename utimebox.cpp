//窗口
#include "utimebox.h"
#include <QHBoxLayout>
//控件
#include <QLabel>
#include <QLineEdit>
//数值
#include <QIntValidator>
#include <QTime>

UTimeBox::UTimeBox(){
    //label
    m_HourLabel = new QLabel("时");
    m_MinLabel = new QLabel("分");
    m_SecLabel = new QLabel("秒");
    m_HourLabel->setFixedWidth(20);
    m_MinLabel->setFixedWidth(20);
    m_SecLabel->setFixedWidth(20);
    m_HourLabel->setAlignment(Qt::AlignCenter);
    m_MinLabel->setAlignment(Qt::AlignCenter);
    m_SecLabel->setAlignment(Qt::AlignCenter);
    m_KongLabel = new QLabel(" ");
    m_KongLabel->setFixedWidth(10);
    //框
    m_HourEdit = new QLineEdit();
    m_HourEdit->setValidator(new QIntValidator(0, 23, m_HourEdit));
    m_HourEdit->setFixedWidth(30);
    m_HourEdit->setMaxLength(2);
    m_MinEdit = new QLineEdit();
    m_MinEdit->setValidator(new QIntValidator(0, 59, m_MinEdit));
    m_MinEdit->setFixedWidth(30);
    m_MinEdit->setMaxLength(2);
    m_SecEdit = new QLineEdit();
    m_SecEdit->setValidator(new QIntValidator(0, 59, m_SecEdit));
    m_SecEdit->setFixedWidth(30);
    m_SecEdit->setMaxLength(2);

    m_HourEdit->setFocusPolicy(Qt::StrongFocus);
    m_MinEdit->setFocusPolicy(Qt::StrongFocus);
    m_SecEdit->setFocusPolicy(Qt::StrongFocus);

    QTime time=QTime::currentTime();
    Update(&time);

    connect(m_HourEdit,&QLineEdit::textChanged,this,&UTimeBox::ChooseHour);
    connect(m_MinEdit,&QLineEdit::textChanged,this,&UTimeBox::ChooseMin);
    connect(m_SecEdit,&QLineEdit::textChanged,this,&UTimeBox::ChooseSec);
    //布局
    m_layout = new QHBoxLayout();
    m_layout->setAlignment(Qt::AlignLeft);
    m_layout->addWidget(m_KongLabel);
    m_layout->addWidget(m_HourEdit);
    m_layout->addWidget(m_KongLabel);
    m_layout->addWidget(m_HourLabel);

    m_layout->addWidget(m_KongLabel);
    m_layout->addWidget(m_MinEdit);
    m_layout->addWidget(m_KongLabel);
    m_layout->addWidget(m_MinLabel);

    m_layout->addWidget(m_KongLabel);
    m_layout->addWidget(m_SecEdit);
    m_layout->addWidget(m_KongLabel);
    m_layout->addWidget(m_SecLabel);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0,0,0,0);
    this->setLayout(m_layout);
    this->setFixedSize(210,25);
}

UTimeBox::~UTimeBox(){    
    delete m_HourLabel;
    delete m_MinLabel;
    delete m_SecLabel;
    delete m_KongLabel;
    delete m_HourEdit;
    delete m_MinEdit;
    delete m_SecEdit;
    delete m_layout;
}

void UTimeBox::Update(){
    QTime temp=QTime::currentTime();
    Update(&temp);
}
void UTimeBox::Update(QTime *time){
    m_HourEdit->setText(QString::number(time->hour()));
    m_MinEdit->setText(QString::number(time->minute()));
    m_SecEdit->setText(QString::number(time->second()));
    ChooseHour(QString::number(time->hour()));
    ChooseMin(QString::number(time->minute()));
    ChooseSec(QString::number(time->second()));
}
void UTimeBox::Update(QString time){
    QString hour,minute,second;
    if(time.length()==6){
        hour=time.left(2);
        minute=time.mid(2,2);
        second=time.right(2);
        m_HourEdit->setText(hour);
        m_MinEdit->setText(minute);
        m_SecEdit->setText(second);
        ChooseHour(hour);
        ChooseMin(minute);
        ChooseSec(second);
    }
    if(time=="Empty"){
        m_HourEdit->setText("");
        m_MinEdit->setText("");
        m_SecEdit->setText("");
        ChooseHour("");
        ChooseMin("");
        ChooseSec("");
    }
}

void UTimeBox::ChooseHour(const QString & hour){m_hour=hour.toInt();}
void UTimeBox::ChooseMin(const QString & min){m_min=min.toInt();}
void UTimeBox::ChooseSec(const QString & sec){m_sec=sec.toInt();}

QString UTimeBox::GetData(){
    if(m_HourEdit->text()=="" || m_MinEdit->text()=="" || m_SecEdit->text()=="")
        return "Empty";
    return QString("%1%2%3").arg(m_hour,2,10,QChar('0')).arg(m_min,2,10,QChar('0')).arg(m_sec,2,10,QChar('0'));
}

int UTimeBox::GetTime(const QString type){
    if(type=="hour")
        return m_hour;
    if(type=="min")
        return m_min;
    if(type=="sec")
        return m_sec;
    return 0;
}
