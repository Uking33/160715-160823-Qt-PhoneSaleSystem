//窗口
#include "uline.h"
#include <QGridLayout>
//控件
#include <QLineEdit>
#include <QLabel>
//辅助
#include <QDoubleValidator>

ULine::ULine(const QString &text, const QString hitText,
             int widthLabel, int heightLabel, int widthEdit, int heightEdit)
{
    m_type=0;
    //设置内容
    m_label = new QLabel(text);
    m_edit = new QLineEdit();
    m_edit->setPlaceholderText(hitText);
    //设置长宽
    m_label->setMaximumSize(widthLabel,heightLabel);
    m_label->setMinimumSize(widthLabel,heightLabel);
    m_edit->setMaximumSize(widthEdit,heightEdit);
    m_edit->setMinimumSize(widthEdit,heightEdit);
    //添加布局
    addWidget(m_label);
    addWidget(m_edit);
    setAlignment(Qt::AlignLeft);
    //连接
    connect(m_edit,&QLineEdit::textEdited,[=](){
        emit textEdited();
    });
}

ULine::~ULine(){
    delete m_label;
    delete m_edit;
}

//值
QString ULine::GetValue(){
    return m_edit->text();
}

void ULine::SetLabel(const QString& text){
    m_label->setText(text);
}

void ULine::SetValue(const QString& value){
    m_edit->setText(value);
}
void ULine::SetValue(const int value){
    m_edit->setText(QString::number(value));
}
void ULine::SetValue(const float value){
    m_edit->setText(QString::number(value));
}
void ULine::ReValue(){
    switch(m_type){
        case 0:
            SetValue("");
            break;
        case 1:
            SetValue("0");
            break;
        case 2:
            SetValue("0");
            break;
    }
}

//设置
void ULine::SetReadOnly(bool isRead){
    m_edit->setReadOnly(isRead);
}
void ULine::SetFocus(){
    m_edit->setFocus();
}

void ULine::SetDouble(int aft,int max,int min){
    if(m_type!=0)
        return;
    m_type=1;    
    int c1=QString::number(max).length()+aft+1;
    int c2=QString::number(min).length()+aft+1;
    int count=c1>c2?c1:c2;
    m_edit->setMaxLength(count);
    connect(m_edit,QLineEdit::textEdited,[=](const QString str){
        if(str.contains("☺") || str.contains(",") || str.contains(" ") || str.contains("%"))
            m_edit->backspace();
        if(str.length()==0)
            SetValue("0");
        if(str.length()==1){
            if(str==".")
                SetValue("0.");
        }
        else if(str.length()==2){
            if(str=="00")
                SetValue("0");
            else if(!str.contains('.') && str.left(1)=="0")
                SetValue(str.right(1));
        }
    });
    QDoubleValidator *validator = new QDoubleValidator(min,max,aft, m_edit);
    validator->setNotation(QDoubleValidator::StandardNotation);
    m_edit->setValidator(validator);
}

void ULine::SetInt(int max,int min){
    if(m_type!=0)
        return;
    m_type=2;
    int c1=QString::number(max).length();
    int c2=QString::number(min).length();
    int count=c1>c2?c1:c2;
    m_edit->setValidator(new QIntValidator(min, max, m_edit));
    m_edit->setMaxLength(count);
    connect(m_edit,QLineEdit::textEdited,[=](const QString str){
        if(str.contains("☺") || str.contains(",") || str.contains(" ") || str.contains("%"))
            m_edit->backspace();
        if(str.length()==0)
            SetValue("0");
        else if(str.length()==2){
            if(str=="00")
                SetValue("0");
            else if(str.left(1)=="0")
                SetValue(str.right(1));
        }
    });
}

void ULine::SetMaxLength(int count){
    m_edit->setMaxLength(count);
    connect(m_edit,&QLineEdit::textEdited,[=](const QString str){
        if(str.contains("☺") || str.contains(",") || str.contains(" ") || str.contains("%"))
            m_edit->backspace();
    });
}

void ULine::SetDoubleNotype(int aft,int max,int min){
    int c1=QString::number(max).length()+aft+1;
    int c2=QString::number(min).length()+aft+1;
    int count=c1>c2?c1:c2;
    m_edit->setMaxLength(count);
    QDoubleValidator *validator = new QDoubleValidator(min,max,aft, m_edit);
    validator->setNotation(QDoubleValidator::StandardNotation);
    m_edit->setValidator(validator);
}

void ULine::SetIntNotype(int max,int min){
    int c1=QString::number(max).length();
    int c2=QString::number(min).length();
    int count=c1>c2?c1:c2;
    m_edit->setValidator(new QIntValidator(min, max, m_edit));
    m_edit->setMaxLength(count);
}

void ULine::SetMaxLengthNotype(int count){
    m_edit->setMaxLength(count);
}


void ULine::SetValidator(QString rect){
    m_edit->setValidator(new QRegExpValidator(QRegExp(rect), m_edit));
}
