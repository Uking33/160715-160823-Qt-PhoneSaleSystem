//窗口
#include "phoneloading.h"
//布局
#include <QHBoxLayout>
//控件
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
//数据
#include "type.h"
//辅助
#include <QTableWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include <QList>
PhoneLoading::PhoneLoading(QTableWidget *table){
    m_table = table;
    //界面
    progressBar = new QProgressBar;
    debug = new QLabel;
    layout = new QHBoxLayout;

    debug->setFixedWidth(100);
    debug->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    layout->addWidget(progressBar);
    layout->addWidget(debug);
    this->setLayout(layout);
}

PhoneLoading::~PhoneLoading(){
    delete layout;
}


void PhoneLoading::Start(QString type,long long count, PhoneData *data,QList<PhoneData*>* dataList, QStringList strList){
    progressBar->setValue(0);

    thread = new PhoneThread(this,m_table,type,data,dataList, strList);
    connect(thread, &PhoneThread::finished, thread, &PhoneThread::deleteLater);
    connect(thread, &PhoneThread::finished, this, &PhoneLoading::close);

    m_count=count;
    m_gress=0;
    connect(thread,&PhoneThread::LoadHit,[=](QString str){
        if(str.contains("过程-")){
            debug->setText(str);
            m_gress+=100.0/m_count;
            progressBar->setValue(m_gress);
        }
    });

    thread->start();
    this->exec();
}

void PhoneLoading::End(){
    this->close();
}

PhoneThread::PhoneThread(QObject *parent,QTableWidget *table,QString type,PhoneData *data,QList<PhoneData*>* dataList,QStringList strList)
        : QThread(parent)
{
    m_table=table;
    m_type=type;
    m_data=data;
    m_dataList=dataList;
    m_strList=strList;
}

void PhoneThread::EDIT(int index){
    QStringList nameList;
    nameList=Type::PHONE_ALL;
    foreach(QString str,m_strList){
        switch(nameList.indexOf(str)){
            case 0:
                m_table->item(index,0)->setText(m_data->IMEI);
                m_dataList->at(index)->IMEI=m_data->IMEI;
                break;
            case 1:
                m_table->item(index,1)->setText(m_data->phoneBrand);
                m_dataList->at(index)->phoneBrand=m_data->phoneBrand;
                break;
            case 2:
                m_table->item(index,2)->setText(m_data->phoneModels);
                m_dataList->at(index)->phoneModels=m_data->phoneModels;
                break;
            case 3:
                m_table->item(index,3)->setText("￥"+QString::number(m_data->buyingPrice,'f',2));
                m_dataList->at(index)->buyingPrice=m_data->buyingPrice;
                break;
            case 4:
                m_table->item(index,4)->setText("￥"+QString::number(m_data->salingPrice,'f',2));
                m_dataList->at(index)->salingPrice=m_data->salingPrice;
                break;
            case 5:
                if(m_data->inTime==""){
                    str="未知";
                    m_table->item(index,5)->setText(str);
                }
                else{
                    str=m_data->inTime;
                    m_table->item(index,5)->setText(str.left(4)+"年"+str.mid(4,2)+"月"+str.mid(6,2)+"日"+str.mid(8,2)+":"+str.mid(10,2)+":"+str.mid(12,2));
               }
               m_dataList->at(index)->inTime=m_data->inTime;
                break;
            case 6:
                if(m_data->outTime==""){
                    str="未出售";
                    m_table->item(index,6)->setText(str);
                }
                else{
                    str=m_data->outTime;
                    m_table->item(index,6)->setText(str.left(4)+"年"+str.mid(4,2)+"月"+str.mid(6,2)+"日"+str.mid(8,2)+":"+str.mid(10,2)+":"+str.mid(12,2));
                }
                m_dataList->at(index)->outTime=m_data->outTime;
                break;
            case 7:
                m_table->item(index,7)->setText(m_data->shopAssistant);
                m_dataList->at(index)->shopAssistant=m_data->shopAssistant;
                break;
            case 8:
                m_table->item(index,8)->setText(m_data->cashier);
                m_dataList->at(index)->cashier=m_data->cashier;
                break;
            case 9:
                m_table->item(index,9)->setText(m_data->addition);
                m_dataList->at(index)->addition=m_data->addition;
                break;
        }
        emit LoadHit(QString("过程-更新"));
    }
}

void PhoneThread::SALE(int index){
    m_table->item(index,6)->setText("已添加");
    emit LoadHit(QString("过程-更新"));
}

void PhoneThread::SALEOUT(int index){
    m_table->item(index,6)->setText("未出售");
    emit LoadHit(QString("过程-更新"));
}

void PhoneThread::DEL(int index){
    m_table->removeRow(index);
    delete m_dataList->at(index);
    m_dataList->removeAt(index);
    emit LoadHit(QString("过程-更新"));
}
void PhoneThread::run() {
    if(m_type=="SALE"){
        for(int index=m_table->rowCount()-1;index>=0;index--)
            if(m_table->item(index,0)!=NULL && m_table->item(index,0)->isSelected())
                QMetaObject::invokeMethod(this, "SALE", Qt::QueuedConnection,Q_ARG(int,index));

    }
    else if(m_type=="SALEOUT"){
        for(int index=m_table->rowCount()-1;index>=0;index--){
            if(m_table->item(index,0)!=NULL && m_strList.contains(m_table->item(index,0)->text()))
                QMetaObject::invokeMethod(this, "SALEOUT", Qt::QueuedConnection,Q_ARG(int,index));
        }

    }
    else if(m_type=="EDIT"){
        for(int index=m_table->rowCount()-1;index>=0;index--)
            if(m_table->item(index,0)!=NULL && m_table->item(index,0)->isSelected())
                QMetaObject::invokeMethod(this, "EDIT", Qt::QueuedConnection,Q_ARG(int,index));
    }
    else if(m_type=="DEL"){
        for(int index=m_table->rowCount()-1;index>=0;index--)
            if(m_table->item(index,0)!=NULL && m_table->item(index,0)->isSelected()){
                QMetaObject::invokeMethod(this, "DEL", Qt::QueuedConnection,Q_ARG(int,index));
            }
    }

}

void PhoneLoading::closeEvent(QCloseEvent *event)
{
    if(this->thread->isRunning()){
        event->ignore();  //忽略退出信号，程序继续运行
        QMessageBox message(QMessageBox::Information, "提示", QString("请等待运行完毕!"));
        message.exec();
    }
}
