//窗口
#include "udatebox.h"
#include <QHBoxLayout>
//控件
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
//数据
#include <QDate>
#include <QCompleter>
#include <QDebug>
UDateBox::UDateBox(int state, UDateBox *lastDateBox){
    m_state=state;
    m_lastDateBox=lastDateBox;
    m_dateMax=NULL;
    //label
    m_label1 = new QLabel("年");
    m_label2 = new QLabel("月");
    m_label3 = new QLabel("日");
    m_label1->setFixedWidth(20);
    m_label2->setFixedWidth(20);
    m_label3->setFixedWidth(20);
    m_label1->setAlignment(Qt::AlignCenter);
    m_label2->setAlignment(Qt::AlignCenter);
    m_label3->setAlignment(Qt::AlignCenter);
    //下拉框
    m_comboBoxYear = new QComboBox();
    m_comboBoxMonth = new QComboBox();
    m_comboBoxDay = new QComboBox();

    m_comboBoxYear->setFixedWidth(50);
    m_comboBoxMonth->setFixedWidth(50);
    m_comboBoxDay->setFixedWidth(50);

    m_comboBoxYear->setFocusPolicy(Qt::NoFocus);
    m_comboBoxMonth->setFocusPolicy(Qt::NoFocus);
    m_comboBoxDay->setFocusPolicy(Qt::NoFocus);

    ConnectBox();
    //初始化
    Update();
    if(m_state==3 && m_lastDateBox!=NULL){
        connect(m_lastDateBox,&UDateBox::textChanged,[=](){
            DisconnectBox();
            m_comboBoxYear->clear();
            for(int i=m_dateMax->year();i>=m_lastDateBox->m_comboBoxYear->currentText().toInt();i--)
                m_comboBoxYear->addItem(QString::number(i));
            m_comboBoxYear->setCurrentIndex(0);
            m_year=m_comboBoxYear->currentText();

            m_comboBoxYear->setCurrentText(m_lastDateBox->m_comboBoxYear->currentText());
            ChooseYear(m_lastDateBox->m_comboBoxYear->currentText());

            if(m_comboBoxYear->currentText()==m_lastDateBox->m_dateMax->toString("yyyy")){
                m_comboBoxMonth->setCurrentText(QString::number(m_dateMax->month()));
                ChooseMonth(QString::number(m_dateMax->month()));
            }
            else{
                m_comboBoxMonth->setCurrentText(QString::number(12));
                ChooseMonth(QString::number(12));
            }

            if(m_comboBoxYear->currentText()==m_lastDateBox->m_dateMax->toString("yyyy") && m_comboBoxMonth->currentText().toInt()==m_lastDateBox->m_dateMax->toString("MM").toInt()){
                m_comboBoxDay->setCurrentText(QString::number(m_dateMax->day()));
                ChooseDay(QString::number(m_dateMax->day()));
            }
            else{
                QDate date(m_year.toInt(),m_month.toInt(),1);
                m_comboBoxDay->setCurrentText(QString::number(date.daysInMonth()));
                ChooseDay(QString::number(date.daysInMonth()));
            }
            ConnectBox();
        });
    }
    //布局
    m_layout = new QHBoxLayout();
    m_layout->setAlignment(Qt::AlignLeft);
    m_layout->addWidget(m_comboBoxYear);
    m_layout->addWidget(m_label1);
    m_layout->addWidget(m_comboBoxMonth);
    m_layout->addWidget(m_label2);
    m_layout->addWidget(m_comboBoxDay);
    m_layout->addWidget(m_label3);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0,0,0,0);
    this->setLayout(m_layout);
    this->setFixedSize(210,25);
}

UDateBox::~UDateBox(){
    DisconnectBox();
    delete m_layout;
    if(m_dateMax!=NULL)
        delete m_dateMax;
}

void UDateBox::Today(bool){
    m_comboBoxYear->setCurrentText(QString::number(m_dateMax->year()));
    m_comboBoxMonth->setCurrentText(QString::number(m_dateMax->month()));
    m_comboBoxDay->setCurrentText(QString::number(m_dateMax->day()));
}

void UDateBox::ToWeek(bool){
    m_comboBoxYear->setCurrentText(QString::number(m_dateMax->year()));
    m_comboBoxMonth->setCurrentText(QString::number(m_dateMax->month()));
    m_comboBoxDay->setCurrentText(QString::number(m_dateMax->daysInMonth()-m_dateMax->dayOfWeek()));
}

void UDateBox::ToMonth(bool){
    m_comboBoxYear->setCurrentText(QString::number(m_dateMax->year()));
    m_comboBoxMonth->setCurrentText(QString::number(m_dateMax->month()));
    m_comboBoxDay->setCurrentText("1");
}


void UDateBox::ToYear(bool){
    m_comboBoxYear->setCurrentText(QString::number(m_dateMax->year()));
    m_comboBoxMonth->setCurrentText("1");
    m_comboBoxDay->setCurrentText("1");
}

void UDateBox::First(bool){
    m_comboBoxYear->setCurrentIndex(0);
    m_comboBoxMonth->setCurrentIndex(0);
    m_comboBoxDay->setCurrentIndex(0);
}

void UDateBox::Update(const QString& date){//正常选中时间
    //清空
    DisconnectBox();
    m_comboBoxYear->clear();
    m_comboBoxMonth->clear();
    m_comboBoxDay->clear();
    if(date.length()==8){
        QString year,month,day;
        year=date.left(4);
        month=QString::number(date.mid(4,2).toInt());
        day=QString::number(date.mid(6,2).toInt());
        //年
        if(m_state==3)
            for(int i=m_dateMax->year();i>=(m_lastDateBox->m_comboBoxYear->currentText().toInt());i--)
                m_comboBoxYear->addItem(QString::number(i));
        else
            for(int i=m_dateMax->year();i>=(m_dateMax->year()-100);i--)
                m_comboBoxYear->addItem(QString::number(i));
        ChooseYear(year);
        m_comboBoxYear->setCurrentText(year);
        ChooseMonth(month);
        m_comboBoxMonth->setCurrentText(month);
        ChooseDay(day);
        m_comboBoxDay->setCurrentText(day);
    }
    else if(date=="Empty"){
        //年
        m_comboBoxYear->addItem("");
        for(int i=m_dateMax->year();i>=(m_dateMax->year()-100);i--)
            m_comboBoxYear->addItem(QString::number(i));
        m_comboBoxYear->setCurrentIndex(0);
        m_state=0;
        m_year="";
        m_month="";
        m_day="";
    }
    ConnectBox();
}

void UDateBox::Update(){
    QDate *tempDate=new QDate;
    (*tempDate)=QDate::currentDate();
    Update(tempDate);
}

void UDateBox::Update(QDate *date){
    int i;
    int n;
    //清空
    DisconnectBox();
    m_comboBoxYear->clear();
    m_comboBoxMonth->clear();
    m_comboBoxDay->clear();
    if(0==m_state){
        if(m_dateMax!=NULL)
            delete m_dateMax;
        m_dateMax=date;
    }
    else if(1==m_state){//普通到今天
        //年
        for(i=date->year();i>=(date->year()-100);i--)
            m_comboBoxYear->addItem(QString::number(i));
        m_comboBoxYear->setCurrentIndex(0);
        m_year=QString::number(date->year());
        //月
        n=date->month();
        for(int i=1;i<=n;i++)
            m_comboBoxMonth->addItem(QString::number(i));
        m_comboBoxMonth->setCurrentIndex(date->month()-1);
        m_month=QString::number(date->month());
        //日
        n=date->day();
        for(i=1;i<=n;i++)
            m_comboBoxDay->addItem(QString::number(i));
        m_comboBoxDay->setCurrentIndex(date->day()-1);
        m_day=QString::number(date->day());
        if(m_dateMax!=NULL)
            delete m_dateMax;
        m_dateMax=date;
    }
    else if(2==m_state){
        //年
        for(i=date->year();i>=(date->year()-100);i--)
            m_comboBoxYear->addItem(QString::number(i));
        m_comboBoxYear->setCurrentIndex(0);
        m_year=QString::number(date->year());
        //月
        for(int i=1;i<=date->month();i++)
            m_comboBoxMonth->addItem(QString::number(i));
        m_comboBoxMonth->setCurrentIndex(date->month()-1);
        m_month=QString::number(date->month());
        //日
        int i;
        for(i=1;i<=date->day();i++)
            m_comboBoxDay->addItem(QString::number(i));
        m_comboBoxDay->setCurrentIndex(0);
        m_day=QString::number(1);
        if(m_dateMax!=NULL)
            delete m_dateMax;
        m_dateMax=date;
    }
    else if(3==m_state){
        //年
        for(i=date->year();i>=(date->year()-100);i--)
            m_comboBoxYear->addItem(QString::number(i));
        m_comboBoxYear->setCurrentIndex(0);
        m_year=QString::number(date->year());
        //月
        for(int i=1;i<=date->month();i++)
            m_comboBoxMonth->addItem(QString::number(i));
        m_comboBoxMonth->setCurrentIndex(date->month()-1);
        m_month=QString::number(date->month());
        //日
        int i;
        for(i=1;i<=date->day();i++)
            m_comboBoxDay->addItem(QString::number(i));
        m_comboBoxDay->setCurrentIndex(0);
        m_day=QString::number(1);
        if(m_dateMax!=NULL)
            delete m_dateMax;
        m_dateMax=date;
    }
    else if(4==m_state){//所有
        //年
        m_comboBoxYear->addItem("所有");
        for(i=date->year();i>=(date->year()-100);i--)
            m_comboBoxYear->addItem(QString::number(i));
        m_comboBoxYear->setCurrentIndex(0);
        m_year="所有";
        //月
        m_comboBoxMonth->addItem("所有");
        n=date->month();
        for(int i=1;i<=n;i++)
            m_comboBoxMonth->addItem(QString::number(i));
        m_comboBoxMonth->setCurrentIndex(0);
        m_month="所有";
        //日
        m_comboBoxDay->addItem("所有");
        n=date->day();
        for(i=1;i<=n;i++)
            m_comboBoxDay->addItem(QString::number(i));
        m_comboBoxDay->setCurrentIndex(0);
        m_day="所有";
        if(m_dateMax!=NULL)
            delete m_dateMax;
        m_dateMax=date;
    }
    ConnectBox();
}

void UDateBox::ChooseYear(const QString& year){
    m_year=year;
    DisconnectBox();
    //清空
    m_comboBoxMonth->clear();
    //添加1
    if(0==m_state)
        m_comboBoxMonth->addItem("");
    if(4==m_state)
        m_comboBoxMonth->addItem("所有");
    else if(2==m_state){
        int n=1;
        if(m_year==m_dateMax->toString("yyyy"))
            n=m_dateMax->month();
        else
            n=12;
        for(int i=1;i<=n;i++)
            m_comboBoxMonth->addItem(QString::number(i));
    }
    else if(3==m_state){
        int n=1,i=1;
        if(m_year==m_dateMax->toString("yyyy"))
            n=m_dateMax->month();
        else
            n=12;
        if(m_year==m_lastDateBox->m_comboBoxYear->currentText())
            i=m_lastDateBox->m_comboBoxMonth->currentText().toInt();
        else
            i=1;
        for(;i<=n;i++)
            m_comboBoxMonth->addItem(QString::number(i));
    }
    //添加2
    if(2!=m_state && 3!=m_state && m_year!="所有"){
        int n=1;
        if(m_year==m_dateMax->toString("yyyy"))
            n=m_dateMax->month();
        else
            n=12;
        for(int i=1;i<=n;i++)
            m_comboBoxMonth->addItem(QString::number(i));
    }
    //下一步
    if(0==m_state){
        m_comboBoxMonth->setCurrentText("");
        ChooseMonth("");
    }
    else if(1==m_state){
        m_comboBoxMonth->setCurrentText("1");
        ChooseMonth("1");
    }
    else if(2==m_state){
        m_comboBoxMonth->setCurrentText("1");
        ChooseMonth("1");
    }
    else if(3==m_state){
        m_comboBoxMonth->setCurrentIndex(m_comboBoxMonth->count()-1);
        ChooseDay(m_comboBoxMonth->itemText(m_comboBoxMonth->count()-1));
    }
    else if(4==m_state){
        m_comboBoxMonth->setCurrentText("所有");
        ChooseMonth("所有");
    }
    ConnectBox();
}

void UDateBox::ChooseMonth(const QString& month){
    int i,n;
    m_month=month;
    DisconnectBox();
    //清空
    m_comboBoxDay->clear();
    //添加
    if(0==m_state)
        m_comboBoxDay->addItem("");
    if(4==m_state)
        m_comboBoxDay->addItem("所有");
    else if(2==m_state){
        if(m_year==m_dateMax->toString("yyyy") && m_month.toInt()==m_dateMax->toString("MM").toInt())
            n=m_dateMax->day();
        else{
            QDate date(m_year.toInt(),m_month.toInt(),1);
            n=date.daysInMonth();
        }
        for(i=1;i<=n;i++)
            m_comboBoxDay->addItem(QString::number(i));
    }
    else if(3==m_state){
        if(m_year==m_lastDateBox->m_dateMax->toString("yyyy") && m_month.toInt()==m_dateMax->toString("MM").toInt())
            n=m_dateMax->day();
        else{
            QDate date(m_year.toInt(),m_month.toInt(),1);
            n=date.daysInMonth();
        }
        if(m_year==m_lastDateBox->m_comboBoxYear->currentText() && m_month.toInt()==m_lastDateBox->m_comboBoxMonth->currentText().toInt())
            i=m_lastDateBox->m_comboBoxDay->currentText().toInt();
        else
            i=1;
        for(;i<=n;i++)
            m_comboBoxDay->addItem(QString::number(i));
    }

    if(2!=m_state && 3!=m_state && m_month!="所有"){
        if(m_year==m_dateMax->toString("yyyy") && m_month.toInt()==m_dateMax->toString("MM").toInt())
            n=m_dateMax->day();
        else{
            QDate date(m_year.toInt(),m_month.toInt(),1);
            n=date.daysInMonth();
        }
        for(i=1;i<=n;i++)
            m_comboBoxDay->addItem(QString::number(i));
    }
    if(0==m_state){
        m_comboBoxDay->setCurrentText("");
        ChooseDay("");
    }
    else if(1==m_state){
        m_comboBoxDay->setCurrentText("1");
        ChooseDay("1");
    }
    else if(2==m_state){
        m_comboBoxDay->setCurrentText("1");
        ChooseDay("1");
    }
    else if(3==m_state){
        m_comboBoxDay->setCurrentIndex(m_comboBoxDay->count()-1);
        ChooseDay(m_comboBoxDay->itemText(m_comboBoxDay->count()-1));
    }
    else if(4==m_state){
        m_comboBoxDay->setCurrentText("所有");
        ChooseDay("所有");
    }
    ConnectBox();
}
void UDateBox::ChooseDay(const QString& day){
    if(day!="New"){
        m_day=day;
        emit textChanged();
    }
}

QString UDateBox::GetData(){
    if(m_year=="" || m_month=="" || m_day=="")
        return "Empty";
    if(m_year=="所有")
        return "ALL";
    if(m_month=="所有")
        return m_year;
    if(m_day=="所有")
        return QString("%1%2").arg(m_year.toInt(),4,10,QChar('0')).arg(m_month.toInt(),2,10,QChar('0'));
    return QString("%1%2%3").arg(m_year.toInt(),4,10,QChar('0')).arg(m_month.toInt(),2,10,QChar('0')).arg(m_day.toInt(),2,10,QChar('0'));
}

QString UDateBox::GetTable(){
    if(m_year=="所有")
        return "ALL";
    return "第"+m_year+"年";
}


int UDateBox::GetDate(const QString type){
    if(type=="year")
        return m_year.toInt();
    if(type=="month")
        return m_month.toInt();
    if(type=="day")
        return m_day.toInt();
    return 0;
}

void UDateBox::DisconnectBox(){
    disconnect(m_comboBoxYear,&QComboBox::currentTextChanged,this,&UDateBox::ChooseYear);
    disconnect(m_comboBoxMonth,&QComboBox::currentTextChanged,this,&UDateBox::ChooseMonth);
    disconnect(m_comboBoxDay,&QComboBox::currentTextChanged,this,&UDateBox::ChooseDay);
}

void UDateBox::ConnectBox(){
    connect(m_comboBoxYear,&QComboBox::currentTextChanged,this,&UDateBox::ChooseYear);
    connect(m_comboBoxMonth,&QComboBox::currentTextChanged,this,&UDateBox::ChooseMonth);
    connect(m_comboBoxDay,&QComboBox::currentTextChanged,this,&UDateBox::ChooseDay);
}
