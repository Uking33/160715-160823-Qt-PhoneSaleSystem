//窗口
#include "urectbox.h"
#include <QHBoxLayout>
//控件
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
//数据
#include <QIntValidator>

URectBox::URectBox(const QString & title,const QString & from,const QString & to)
{    
    //label
    m_titleLabel = new QLabel(title);
    m_fromLabel = new QLabel(from+"到");
    m_toLabel = new QLabel(to);
    m_checkBox = new QCheckBox();
    m_titleLabel->setFixedWidth(50);
    m_checkBox->setFixedWidth(20);
    m_fromLabel->setFixedWidth(40);
    m_toLabel->setFixedWidth(20);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_fromLabel->setAlignment(Qt::AlignCenter);
    m_toLabel->setAlignment(Qt::AlignCenter);
    //框
    m_fromEdit = new QLineEdit();
    m_fromEdit->setValidator(new QIntValidator(0, 999999, m_fromEdit));
    m_fromEdit->setFixedWidth(50);
    m_fromEdit->setMaxLength(6);
    m_toEdit = new QLineEdit();
    m_toEdit->setValidator(new QIntValidator(0, 999999, m_toEdit));
    m_toEdit->setFixedWidth(50);
    m_toEdit->setMaxLength(6);

    m_fromEdit->setFocusPolicy(Qt::StrongFocus);
    m_toEdit->setFocusPolicy(Qt::StrongFocus);

    Update();
    m_checkBox->setFocusPolicy(Qt::NoFocus);
    connect(m_checkBox,&QCheckBox::stateChanged,this,&URectBox::stateChanged);

    QObject::connect(m_fromEdit,&QLineEdit::editingFinished,[=](){
        QString str=m_fromEdit->text();
        if(str.toLongLong()>m_to)
            m_fromEdit->setText(QString::number(m_from));
        else{
            m_from=str.toLongLong();
            m_fromEdit->setText(QString::number(m_from));
        }
    });
    QObject::connect(m_toEdit,&QLineEdit::editingFinished,[=](){
        QString str=m_toEdit->text();
        if(str.toLongLong()<m_from)
            m_toEdit->setText(QString::number(m_to));
        else{
            m_to=str.toLongLong();
            m_toEdit->setText(QString::number(m_to));
        }
    });
    //布局
    m_layout = new QHBoxLayout();
    m_layout->setAlignment(Qt::AlignLeft);
    m_layout->addWidget(m_titleLabel);
    m_layout->addWidget(m_checkBox);
    m_layout->addWidget(m_fromEdit);
    m_layout->addWidget(m_fromLabel);
    m_layout->addWidget(m_toEdit);
    m_layout->addWidget(m_toLabel);

    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0,0,0,0);
    this->setLayout(m_layout);
    this->setFixedSize(230,25);
}

URectBox::~URectBox(){
    delete m_checkBox;
    delete m_titleLabel;
    delete m_fromLabel;
    delete m_toLabel;
    delete m_fromEdit;
    delete m_toEdit;
    delete m_layout;
}

long long URectBox::GetFrom(){
    if(m_from!=-1 && m_fromEdit->text()=="")
        m_fromEdit->setText(QString::number(m_from));
    if(m_fromEdit->text()!="" && m_fromEdit->text().toLongLong()!=m_from){
        QString str=m_fromEdit->text();
        if(str.toLongLong()>m_to)
            m_fromEdit->setText(QString::number(m_from));
        else{
            m_from=str.toLongLong();
            m_fromEdit->setText(QString::number(m_from));
        }
    }
    return m_from;
}

long long URectBox::GetTo(){
    if(m_to!=-1 && m_toEdit->text()=="")
        m_toEdit->setText(QString::number(m_to));
    if(m_toEdit->text()!="" && m_toEdit->text().toLongLong()!=m_to){
        QString str=m_toEdit->text();
        if(str.toLongLong()<m_from)
            m_toEdit->setText(QString::number(m_to));
        else{
            m_to=str.toLongLong();
            m_toEdit->setText(QString::number(m_to));
        }
    }
    return m_to;
}

void URectBox::Update(){
    stateChanged(0);
}

void URectBox::stateChanged(int state){
    switch(state){
        case 0:
            m_fromEdit->setEnabled(false);
            m_toEdit->setEnabled(false);
            m_fromEdit->setText("");
            m_toEdit->setText("");
            m_from=-1;
            m_to=-1;
            break;
        case 2:
            m_fromEdit->setEnabled(true);
            m_toEdit->setEnabled(true);
            m_from=0;
            m_to=10000;
            m_fromEdit->setText("0");
            m_toEdit->setText("10000");
            break;
    }
}

void URectBox::SetShortcut(QString key){
    m_checkBox->setShortcut(QKeySequence(key));
}
