#include "data.h"
#include <QDebug>
#include <QApplication>
#include <QtCore>
//数据库
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
//时间
#include <QDateTime>
#include <QTime>

Data::Data(){
    InitDatabase();
}

Data::~Data(){
}

/*一级操作*/
//--初始化--
bool Data::OpenDatabase(const QString &path){
    //数据库-连接数据库-添加驱动
    m_database = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    m_database->setDatabaseName(path);    //数据库名
    m_database->setPassword("123456");        //数据库密码
    //数据库-连接数据库-发送连接
    if (!m_database->open())
        return false;
    emit LoadHit("打开数据库-已打开");
    return true;
}
bool Data::CloseDatabase(){
    delete m_query;
    m_database->close();
    delete m_database;
    emit LoadHit("关闭数据库-已关闭");
    return true;
}
bool Data:: OpenTable(QString path,QString table){
    emit LoadHit("连接数据库-准备表单连接");
    //数据库-连接数据库
    if(!OpenDatabase(path)){
        SendMessageHit("打开数据库-打开错误 "+table);
        return false;
    }
    //数据库-检查创建表
    m_query = new QSqlQuery(*m_database);
    if(path=="phoneDatabase.db")
        m_query->prepare(QString("create table %1 (IMEI nvarchar(15), phoneBrand nvarchar(30), phoneModels nvarchar(30), buyingPrice float, salingPrice float,"
                                 "addition nvarchar, inTime nvarchar(14), outTime nvarchar(14), shopAssistant nvarchar(30), cashier nvarchar(30))").arg(table));
    else if(path=="recordDatabase.db")
        m_query->prepare(QString("create table %1 (ID nvarchar(12), outTime nvarchar(15), shopAssistant nvarchar(30), cashier nvarchar(30), price float, gather float, unSale nvarchar(16),"
                                 "IMEI nvarchar, phoneModels nvarchar, buyingPrice nvarchar, salingPrice nvarchar, unit nvarchar, count nvarchar)").arg(table));
    else if(path=="countDatabase.db"){
        if(table=="Tool")
            m_query->prepare(QString("create table %1 (name nvarchar(30), count int, buyingPrice float, salingPrice float, unit nvarchar(10))").arg(table));
        else if(table=="Assets")
            m_query->prepare(QString("create table %1 (name nvarchar(30), count int, price float, addition nvarchar(1000))").arg(table));
        path+=table;
    }
    else if(path=="errorDatabase.db")
            m_query->prepare(QString("create table %1 (ID nvarchar(12), name nvarchar(50), date nvarchar(8), reason nvarchar(100), price float, unSale nvarchar(16), list nvarchar)").arg(table));
    else if(path=="staffDatabase.db"){
        if(table=="☺ALL☺")
            m_query->prepare(QString("create table %1 (ID nvarchar(5), password nvarchar(15), name nvarchar(30), inDate nvarchar(8), addition nvarchar(1000))").arg(table));
        else if(table.right(5)=="☺REC☺")
            m_query->prepare(QString("create table %1 (ID nvarchar(12), date nvarchar(8), work nvarchar(15),"
                                     " price float, gather float, unSale nvarchar(16))").arg(table));
        else if(table.right(5)=="☺ERR☺")
            m_query->prepare(QString("create table %1 (ID nvarchar(12), name nvarchar(30), date nvarchar(8),"
                                     " reason nvarchar, price float, unSale nvarchar(16))").arg(table));
        path+=table;
    }
    else if(path=="wageDatabase.db")
        m_query->prepare(QString("create table %1 (IDname nvarchar(38), startDate nvarchar(8), endDate nvarchar(8), wage float, base float, gather float, per float,"
                                 "errorID nvarchar, name nvarchar, reason nvarchar, date nvarchar, price nvarchar)").arg(table));
    if(m_query->exec()){
        emit LoadHit("连接数据库-表单创建成功");
    }
    else{
        emit LoadHit("连接数据库-检查表单");
        //数据库-检查创建表-表已存在-检测预处理
        m_query->exec(QString("select * from %1").arg(table));
        QSqlRecord rec = m_query->record();
        //数据库-检查创建表-表已存在-检测字段数
        if(rec.count()!=m_col[path]){
            qDebug()<<QString("连接数据库-表单不完整:字段数 "+path+"-"+table+":"+rec.count());
            return false;
        }
        //数据库-检查创建表-表已存在-检测字段名称
        QStringList selectList=m_map[path];
        foreach(QString str,selectList){
            if(rec.fieldName(selectList.indexOf(str)) != str){
                qDebug()<<QString("数据库-表单不完整:%1 %2").arg(table).arg(str);
                return false;
            }
        }
        emit LoadHit("连接数据库-表单完整");
    }
    emit LoadHit("连接数据库-表单已连接");
    return true;
}
//--获取--
//获取数据链表-手机
QList<PhoneData*>* Data::GetPhoneData(const QString& table, const QString& value){
    //数据链表
    QList<PhoneData*>*  m_dataList=new QList<PhoneData*>();
    //查询数据
    QString str;
    str=QString("select * from %1").arg(table)+value;
    if(!m_query->exec(str)){
        qDebug()<<m_query->lastError();
        return NULL;
    }
    while(m_query->next()){
        PhoneData *data=new PhoneData();
        data->IMEI=m_query->value("IMEI").toString();
        data->phoneBrand=m_query->value("phoneBrand").toString();
        data->phoneModels=m_query->value("phoneModels").toString();
        data->buyingPrice=m_query->value("buyingPrice").toFloat();
        data->salingPrice=m_query->value("salingPrice").toFloat();
        data->addition=m_query->value("addition").toString();
        data->inTime=m_query->value("inTime").toString();
        data->outTime=m_query->value("outTime").toString();
        data->shopAssistant=m_query->value("shopAssistant").toString();
        data->cashier=m_query->value("cashier").toString();
        m_dataList->append(data);
        emit LoadHit(QString("过程-查找手机"));
    }
    return m_dataList;
}
//获取数据链表-销售记录
QList<PhoneRecordList*>* Data::GetRecordData(const QString& table, const QString& value){
    //数据链表
    QList<PhoneRecordList*>*  m_dataList=new QList<PhoneRecordList*>();
    //查询数据
    QString str;
    int i;
    str=QString("select * from %1").arg(table)+value;
    if(!m_query->exec(str)){
        qDebug()<<m_query->lastError();
        return NULL;
    }
    while(m_query->next()){
        PhoneRecordList *data=new PhoneRecordList();
        data->ID=m_query->value("ID").toString();
        data->outTime=m_query->value("outTime").toString();
        data->shopAssistant=m_query->value("shopAssistant").toString();
        data->cashier=m_query->value("cashier").toString();
        data->price=m_query->value("price").toFloat();
        data->gather=m_query->value("gather").toDouble();
        data->unSale=m_query->value("unSale").toString();

        data->dataList= new QList<PhoneRecord*>();
        PhoneRecord *t_list;
        str=m_query->value("IMEI").toString();
        for(i=0;i<str.count("☺")+1;i++){
            t_list = new PhoneRecord();
            t_list->IMEI=str.section("☺",i,i);
            data->dataList->append(t_list);
        }
        str=m_query->value("phoneModels").toString();
        for(i=0;i<str.count("☺")+1;i++)
            data->dataList->at(i)->phoneModels=str.section("☺",i,i);
        str=m_query->value("buyingPrice").toString();
        for(i=0;i<str.count("☺")+1;i++)
            data->dataList->at(i)->buyingPrice=str.section("☺",i,i).toFloat();
        str=m_query->value("salingPrice").toString();
        for(i=0;i<str.count("☺")+1;i++)
            data->dataList->at(i)->salingPrice=str.section("☺",i,i).toFloat();
        str=m_query->value("unit").toString();
        for(i=0;i<str.count("☺")+1;i++)
            data->dataList->at(i)->unit=str.section("☺",i,i);
        str=m_query->value("count").toString();
        for(i=0;i<str.count("☺")+1;i++)
            data->dataList->at(i)->count=str.section("☺",i,i).toInt();
        m_dataList->append(data);
        emit LoadHit(QString("过程-查找销售记录"));
    }
    return m_dataList;
}
//获取数据链表-员工记录-销售
QList<StaffRec*>* Data::GetStaffRec(const QString& table,QString addition){
    //数据链表
    QList<StaffRec*>*  dataList=new QList<StaffRec*>();
    //查询数据
    QString str;
    str=QString("select * from %1%2").arg(table).arg(addition);
    if(!m_query->exec(str)){
        qDebug()<<m_query->lastError();
        return NULL;
    }
    while(m_query->next()){
        StaffRec *data=new StaffRec();
        data->ID=m_query->value("ID").toString();
        data->date=m_query->value("date").toString();
        data->work=m_query->value("work").toString();
        data->price=m_query->value("price").toDouble();
        data->gather=m_query->value("gather").toDouble();
        data->unSale=m_query->value("unSale").toString();
        dataList->append(data);
        emit LoadHit(QString("过程-查找员工记录-销售"));
    }
    return dataList;
}
//获取数据链表-员工记录-分摊
QList<StaffError*>* Data::GetStaffError(const QString& table,QString addition){
    //数据链表
    QList<StaffError*>*  dataList=new QList<StaffError*>();
    //查询数据
    QString str;
    str=QString("select * from %1%2").arg(table).arg(addition);
    if(!m_query->exec(str)){
        qDebug()<<m_query->lastError();
        return NULL;
    }
    while(m_query->next()){
        StaffError *data=new StaffError();
        data->ID=m_query->value("ID").toString();
        data->name=m_query->value("name").toString();
        data->date=m_query->value("date").toString();
        data->reason=m_query->value("reason").toString();
        data->price=m_query->value("price").toDouble();
        data->unSale=m_query->value("unSale").toString();
        dataList->append(data);
        emit LoadHit(QString("过程-查找员工记录-分摊"));
    }
    return dataList;
}
//获取分摊费用-记录
QList<ErrorData*>* Data::GetErrorData(const QString& table, const QString& value){
    //数据链表
    QList<ErrorData*>*  m_dataList=new QList<ErrorData*>();
    //查询数据
    QString str;
    str=QString("select * from %1").arg(table)+value;
    if(!m_query->exec(str)){
        qDebug()<<m_query->lastError()<<m_query->lastQuery();
        return NULL;
    }
    while(m_query->next()){
        ErrorData *data=new ErrorData();
        data->ID=m_query->value("ID").toString();
        data->name=m_query->value("name").toString();
        data->date=m_query->value("date").toString();
        data->reason=m_query->value("reason").toString();
        data->price=m_query->value("price").toDouble();
        data->unSale=m_query->value("unSale").toString();
        str=m_query->value("list").toString();
        data->staffIDname.clear();
        data->staffPrice.clear();
        if(str!=""){
            QString t_str;
            for(int i=0;i<str.count("☺")+1;i++){
                t_str=str.section("☺",i,i);
                data->staffIDname.append(t_str.section("-",0,0));
                data->staffPrice.append(t_str.section("-",1,1).toDouble());
            }
        }
        m_dataList->append(data);
        emit LoadHit(QString("过程-查找"));
    }
    return m_dataList;
}
//获取工资记录
QList<WageData*>* Data::GetWageData(const QString& table, const QString& value){
    //数据链表
    QList<WageData*>*  m_dataList=new QList<WageData*>();
    //查询数据
    QString str1,str2,str3,str4,str5;
    str1=QString("select * from %1").arg(table)+value;
    if(!m_query->exec(str1)){
        qDebug()<<m_query->lastError();
        return NULL;
    }
    while(m_query->next()){
        WageData *data=new WageData();
        data->IDname=m_query->value("IDname").toString();
        data->startDate=m_query->value("startDate").toString();
        data->endDate=m_query->value("endDate").toString();
        data->wage=m_query->value("wage").toDouble();
        data->base=m_query->value("base").toDouble();
        data->gather=m_query->value("gather").toDouble();
        data->per=m_query->value("per").toDouble();

        str1=m_query->value("errorID").toString();
        str2=m_query->value("name").toString();
        str3=m_query->value("reason").toString();
        str4=m_query->value("date").toString();
        str5=m_query->value("price").toString();
        data->rewardList.clear();
        if(str2!="")
        for(int i=0;i<str1.count("☺")+1;i++){
            RewordData reword;
            reword.errorID=str1.section("☺",i,i);
            reword.name=str2.section("☺",i,i);
            reword.reason=str3.section("☺",i,i);
            reword.date=str4.section("☺",i,i);
            reword.price=str5.section("☺",i,i).toDouble();
            data->rewardList.append(reword);
        }
        m_dataList->append(data);
        emit LoadHit(QString("过程-查找"));
    }
    return m_dataList;
}

//--添加--
bool Data::InsertPhoneData(const QString table,PhoneData *phoneData){
    //插入数据
    m_query->prepare(QString("insert into %1 values(:IMEI, :phoneBrand, :phoneModels, :buyingPrice, :salingPrice,"
                             ":addition, :inTime, :outTime, :shopAssistant, :cashier)").arg(table));
    m_query->addBindValue(phoneData->IMEI);
    m_query->addBindValue(phoneData->phoneBrand);
    m_query->addBindValue(phoneData->phoneModels);
    m_query->addBindValue(phoneData->buyingPrice);
    m_query->addBindValue(phoneData->salingPrice);
    m_query->addBindValue(phoneData->addition);
    m_query->addBindValue(phoneData->inTime);
    m_query->addBindValue(phoneData->outTime);
    m_query->addBindValue(phoneData->shopAssistant);
    m_query->addBindValue(phoneData->cashier);
    if(!m_query->exec()){
        emit InsertError(table,"添加过程");
        qDebug()<<"数据库-插入失败-添加过程";
        return false;
    }
    qDebug()<<QString("数据库-插入成功 %1").arg(phoneData->IMEI);
    return true;
}

bool Data::InsertRecordData(const QString table,PhoneRecordList *record){
    //插入数据
    m_query->prepare(QString("insert into %1 values(:ID, :outTime, :shopAssistant, :cashier, :price, :gather, :unSale, "
                             ":IMEI, :phoneModels, :buyingPrice, :salingPrice, :unit, :count)").arg(table));
    m_query->addBindValue(record->ID);
    m_query->addBindValue(record->outTime);
    m_query->addBindValue(record->shopAssistant);
    m_query->addBindValue(record->cashier);
    m_query->addBindValue(record->price);
    m_query->addBindValue(record->gather);
    m_query->addBindValue("00");
    QString str[6];
    foreach(PhoneRecord *data,(*record->dataList)){
        str[0].append("☺"+data->IMEI);
        str[1].append("☺"+data->phoneModels);
        str[2].append("☺"+QString::number(data->buyingPrice));
        str[3].append("☺"+QString::number(data->salingPrice));
        str[4].append("☺"+data->unit);
        str[5].append("☺"+QString::number(data->count));
    }
    if(str[0]!=""){
        str[0].remove(0,1);
        str[1].remove(0,1);
        str[2].remove(0,1);
        str[3].remove(0,1);
        str[4].remove(0,1);
        str[5].remove(0,1);
    }
    m_query->addBindValue(str[0]);
    m_query->addBindValue(str[1]);
    m_query->addBindValue(str[2]);
    m_query->addBindValue(str[3]);
    m_query->addBindValue(str[4]);
    m_query->addBindValue(str[5]);
    if(!m_query->exec()){
        emit InsertError(table,"添加过程");
        qDebug()<<"数据库-插入失败-添加过程";
        return false;
    }
    qDebug()<<QString("数据库-插入成功 %1").arg(record->ID);
    return true;
}

bool Data::InsertToolData(const QString table, ToolData *tool){
    //插入数据
    m_query->prepare(QString("insert into %1 values(:name, :count ,:buyingPrice, :salingPrice, :unit)").arg(table));
    m_query->addBindValue(tool->name);
    m_query->addBindValue(tool->count);
    m_query->addBindValue(tool->buyingPrice);
    m_query->addBindValue(tool->salingPrice);
    m_query->addBindValue(tool->unit);
    if(!m_query->exec()){
        qDebug()<<"数据库-插入失败-添加过程";
        return false;
    }
    qDebug()<<QString("数据库-插入成功 %1").arg(tool->name);
    return true;
}

bool Data::InsertStaffData(const QString table,StaffDataList *staffDataList){
    //插入数据1
    m_query->prepare(QString("insert into %1 values(:ID, :password, :name, :inTime, :addition)").arg(table));
    m_query->addBindValue(staffDataList->ID);
    m_query->addBindValue(staffDataList->ID);
    m_query->addBindValue(staffDataList->name);
    m_query->addBindValue(staffDataList->inDate);
    m_query->addBindValue(staffDataList->addition);
    if(!m_query->exec()){
        emit InsertError(table,"添加过程");
        qDebug()<<"数据库-插入失败-添加过程"<<m_query->lastQuery()<<m_query->lastError();
        return false;
    }
    qDebug()<<QString("数据库-插入成功 %1").arg(staffDataList->ID);
    return true;
}

bool Data::InsertStaffData(const QString table,StaffRec *staffRec){
    //插入数据
    m_query->prepare(QString("insert into %1 values(:ID, :date, :work, :price, :gather, :unSale)").arg(table));
    m_query->addBindValue(staffRec->ID);
    m_query->addBindValue(staffRec->date);
    m_query->addBindValue(staffRec->work);
    m_query->addBindValue(staffRec->price);
    m_query->addBindValue(staffRec->gather);
    m_query->addBindValue("00");
    if(!m_query->exec()){
        emit InsertError(table,"添加过程");
        qDebug()<<"数据库-插入失败-添加过程"<<table<<staffRec->ID<<m_query->lastError();
        return false;
    }
    qDebug()<<QString("数据库-插入成功 %1 %2").arg(table).arg(staffRec->ID);
    return true;
}

bool Data::InsertStaffData(const QString table,StaffError *staffError){
    //插入数据
    m_query->prepare(QString("insert into %1 values(:ID, :name, :date, :reason, :price, :unSale)").arg(table));
    m_query->addBindValue(staffError->ID);
    m_query->addBindValue(staffError->name);
    m_query->addBindValue(staffError->date);
    m_query->addBindValue(staffError->reason);
    m_query->addBindValue(staffError->price);
    m_query->addBindValue("00");
    if(!m_query->exec()){
        emit InsertError(table,"添加过程");
        qDebug()<<"数据库-插入失败-添加过程";
        return false;
    }
    qDebug()<<QString("数据库-插入成功 %1").arg(staffError->ID);
    return true;
}

bool Data::InsertErrorData(const QString table,ErrorData *error){
    //插入数据
    m_query->prepare(QString("insert into %1 values(:ID, :name, :date, :reason, :price, :unSale, :list)").arg(table));
    m_query->addBindValue(error->ID);
    m_query->addBindValue(error->GetData("write","name"));
    m_query->addBindValue(error->GetData("write","date"));
    m_query->addBindValue(error->GetData("write","reason"));
    m_query->addBindValue(error->GetData("write","price"));
    m_query->addBindValue(error->unSale);
    m_query->addBindValue(error->GetData("write","list"));
    if(!m_query->exec()){
        qDebug()<<"数据库-插入失败-添加过程-分摊";
        return false;
    }
    qDebug()<<QString("数据库-插入成功-分摊 %1").arg(error->name);
    return true;
}

bool Data::InsertWageData(const QString table,WageData *wage){
    //插入数据
    m_query->prepare(QString("insert into %1 values(:IDname, :startDate, :endDate, :wage, :base, :gather, :per, "
                             ":errorID, :name, :reason, :date, :price)").arg(table));
    m_query->addBindValue(wage->IDname);
    m_query->addBindValue(wage->startDate);
    m_query->addBindValue(wage->endDate);
    m_query->addBindValue(wage->wage);
    m_query->addBindValue(wage->base);
    m_query->addBindValue(wage->gather);
    m_query->addBindValue(wage->per);
    QString str;
    str.clear();
    foreach(RewordData reword,wage->rewardList){
        str+=reword.errorID+"☺";
    }
    m_query->addBindValue(str.remove(str.length()-1,1));

    str.clear();
    foreach(RewordData reword,wage->rewardList){
        str+=reword.name+"☺";
    }
    m_query->addBindValue(str.remove(str.length()-1,1));

    str.clear();
    foreach(RewordData reword,wage->rewardList){
        str+=reword.reason+"☺";
    }
    m_query->addBindValue(str.remove(str.length()-1,1));

    str.clear();
    foreach(RewordData reword,wage->rewardList){
        str+=reword.date+"☺";
    }
    m_query->addBindValue(str.remove(str.length()-1,1));

    str.clear();
    foreach(RewordData reword,wage->rewardList){
        str+=QString::number(reword.price)+"☺";
    }
    m_query->addBindValue(str.remove(str.length()-1,1));

    if(!m_query->exec()){
        qDebug()<<"数据库-插入失败-添加过程-工资";
        return false;
    }
    qDebug()<<QString("数据库-工资-插入成功 %1").arg(wage->IDname);
    return true;
}

bool Data::InsertAssetData(const QString table,AssetsData *asset){
    //插入数据
    m_query->prepare(QString("insert into %1 values(:name, :count, :price, :addition)").arg(table));
    m_query->addBindValue(asset->name);
    m_query->addBindValue(asset->count);
    m_query->addBindValue(asset->price);
    m_query->addBindValue(asset->addition);
    if(!m_query->exec()){
        qDebug()<<"数据库-插入失败-添加过程-固定资产"<<table;
        return false;
    }
    qDebug()<<QString("数据库-固定资产-插入成功 %1").arg(asset->name);
    return true;
}

//删除
bool Data::DeleteData(const QString table, const QString IDName, const QString ID){   //删除一条数据
    if(!m_query->exec(QString("delete from %1 where %2='%3'").arg(table).arg(IDName).arg(ID)))
        return false;
    emit LoadHit(QString("过程-删除"));
    return true;
}

//查找是否存在
short Data::SearchData(QString table,QString details,QString value){
    if(!m_query->exec(QString("select *from %1 where %2='%3'").arg(table).arg(details).arg(value)))
    {
        qDebug()<<"查找失败"<<m_query->lastError();
        return -1;
    }
    if(!m_query->next())
        return 0;
    return 1;
}
//查询长度
long long Data::GetLength(QString table,QString addition){
    long long max_id=0;
    m_query->prepare(QString("select count(*) from %1%2").arg(table).arg(addition));
    m_query->exec();
    if(m_query->next())
        max_id=m_query->value(0).toLongLong();
    return max_id;
}
//查询最大id
QString Data::GetMaxID(QString table,QString idname){
    QString max_id = 0;
    if(!m_query->exec(QString("select max(%2) from %1").arg(table).arg(idname)))
    {
        qDebug()<<m_query->lastError();
        emit DataError("Database-GetMaxID",table+idname);
        return "error";
    }
    while(m_query->next())
        max_id = m_query->value(0).toString();
    return max_id;
}
//获取工号
QString Data::GetStaffID(QString staffName){
    if(!m_query->exec(QString("select ID from %1 where %2='%3'").arg("☺ALL☺").arg("name").arg(staffName))){
        qDebug()<<"查找失败"<<m_query->lastError();
        return "-1";
    }
    if(!m_query->next())
        return "0";
    return m_query->value(0).toString();
}
//获取姓名
QString Data::GetStaffName(QString staffID){
    if(!m_query->exec(QString("select name from %1 where %2='%3'").arg("☺ALL☺").arg("ID").arg(staffID))){
        qDebug()<<"查找失败"<<m_query->lastError();
        return "-1";
    }
    if(!m_query->next())
        return "0";
    return m_query->value(0).toString();
}


/*---二级操作---*/
//获取时间（进阶：网络时间）
QString Data::GetTimeDataStr(){
    QDateTime currentDateTime = QDateTime::currentDateTime();
    return QString("%1%2%3%4%5%6").arg(currentDateTime.date().year(),4,10,QChar('0')).arg(currentDateTime.date().month(),2,10,QChar('0')).arg(currentDateTime.date().day(),2,10,QChar('0'))
            .arg(currentDateTime.time().hour(),2,10,QChar('0')).arg(currentDateTime.time().minute(),2,10,QChar('0')).arg(currentDateTime.time().second(),2,10,QChar('0'));
}

int Data::GetTimeData(QString type){
    QStringList tpypeList;
    tpypeList<<"year"<<"month"<<"day"<<"hour"<<"minute"<<"second"<<"msec";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    switch(tpypeList.indexOf(type)){
        case 0:
            return currentDateTime.date().year();
        case 1:
            return currentDateTime.date().month();
        case 2:
            return currentDateTime.date().day();
        case 3:
            return currentDateTime.time().hour();
        case 4:
            return currentDateTime.time().minute();
        case 5:
            return currentDateTime.time().second();
        case 6:
            return currentDateTime.time().msec();
    }
    return -1;
}

//--初始化--
bool Data::InitDatabase(){//只运行一次
    //数据初始化
    QStringList list;
    list<< "IMEI"<< "phoneBrand"<< "phoneModels"<< "buyingPrice"<< "salingPrice"
        <<"addition"<< "inTime"<< "outTime"<< "shopAssistant"<< "cashier";
    m_map.insert("phoneDatabase.db",list);
    list.clear();
    list<< "ID"<< "outTime"<< "shopAssistant"<< "cashier"<< "price"<< "gather"<< "unSale"
        <<"IMEI"<< "phoneModels"<< "buyingPrice"<< "salingPrice"<< "unit"<< "count";
    m_map.insert("recordDatabase.db",list);
    list.clear();
    list<< "name"<< "count"<< "buyingPrice"<< "salingPrice"<<"unit";
    m_map.insert("countDatabase.dbTool",list);
    list.clear();
    list<< "name"<< "count"<<"price"<< "addition";
    m_map.insert("countDatabase.dbAssets",list);
    list.clear();
    list<< "ID"<< "password"<< "name"<< "inDate"<< "addition";
    m_map.insert("staffDatabase.db☺ALL☺",list);
    list.clear();
    list<< "ID"<< "date"<< "work"<< "price"<< "gather"<<"unSale";
    m_map.insert("staffDatabase.db☺REC☺",list);
    list.clear();
    list<< "ID"<< "name"<< "date"<< "reason"<< "price"<<"unSale";
    m_map.insert("staffDatabase.db☺ERR☺",list);
    list.clear();
    list<< "ID"<< "name"<< "date"<< "reason"<< "price"<<"unSale"<< "list";
    m_map.insert("errorDatabase.db",list);
    list.clear();
    list<< "IDname"<< "startDate"<< "endDate"<< "wage"<< "base"<< "gather"<< "per"
           << "errorID"<< "name"<< "reason"<< "date"<< "price";
    m_map.insert("wageDatabase.db",list);

    m_col.insert("phoneDatabase.db",10);
    m_col.insert("recordDatabase.db",13);
    m_col.insert("countDatabase.dbTool",5);
    m_col.insert("countDatabase.dbAssets",4);
    m_col.insert("staffDatabase.db☺ALL☺",5);
    m_col.insert("staffDatabase.db☺REC☺",6);
    m_col.insert("staffDatabase.db☺ERR☺",6);
    m_col.insert("errorDatabase.db",7);
    m_col.insert("wageDatabase.db",12);
    //员工数据库检测
    QString table="☺ALL☺";
    if(!OpenTable("staffDatabase.db",table))
        return false;
    if(!SearchData(table,"ID","0000")){
        StaffDataList *staff=new StaffDataList();
        staff->ID="0000";
        staff->name="管理员";
        staff->inDate="19000101";
        staff->addition="";
        if(!InsertStaffData(table, staff))
            return false;
        CloseDatabase();//关闭
        if(!OpenTable("staffDatabase.db","☺"+staff->ID+"☺"+staff->name+"☺REC☺"))
            return false;
        CloseDatabase();//关闭
        if(!OpenTable("staffDatabase.db","☺"+staff->ID+"☺"+staff->name+"☺ERR☺"))
            return false;
        delete staff;
    }
    CloseDatabase();//关闭
    return true;
}

//--获取--
//获取数据：手机、记录表、分摊、工资
void Data::GetDatabase(QString path, QString table, QString addition){
    //定义1
    QList<PhoneData*>* dataList=NULL;
    QList<PhoneRecordList*>* recordList=NULL;
    QList<ErrorData*>* errorList=NULL;
    QList<WageData*>* wageList=NULL;
    //全部
    if(table=="ALL"){
        //定义2
        QList<PhoneData*>* t_dataList;
        QList<PhoneRecordList*>* t_recordList;
        QList<ErrorData*>* t_errorList;
        QList<WageData*>* t_wageList;
        if(path=="phoneDatabase.db")
            dataList = new QList<PhoneData*>();
        else if(path=="recordDatabase.db")
            recordList = new QList<PhoneRecordList*>();
        else if(path=="errorDatabase.db")
            errorList = new QList<ErrorData*>();
        else if(path=="wageDatabase.db")
            wageList = new QList<WageData*>();
        QStringList* tableList=new QStringList();
        //数据库-连接数据库
        if(!OpenDatabase(path)){
            qFatal("failed to connect.") ;
            emit SendMessageHit("数据库-数据库打开错误 "+path);
            return;
        }
        m_query = new QSqlQuery(*m_database);
        //获取所有表名
        if(!m_query->exec(QString("select name from sqlite_master where type='table' order by name"))){
            emit SendMessageHit("数据库-查询所有表错误 "+path);
            return;
        }
        while (m_query->next())
            tableList->append(m_query->value(0).toString());
        //获取长度
        long long sum=0;
        long long now;
        foreach(QString tableStr,*tableList){
            now=GetLength(tableStr,addition);
            if(now==-1){
                sum=-1;
                break;
            }
            sum+=now;
        }
        if(sum<=-1){
            if(path=="phoneDatabase.db")
                emit SendPhoneData(NULL);
            else if(path=="recordDatabase.db")
                emit SendRecordData(NULL);
            else if(path=="errorDatabase.db")
                emit SendErrorData(NULL);
            else if(path=="wageatabase.db")
                emit SendWageData(NULL);
            emit SendMessageHit("未找到记录");
            CloseDatabase();//关闭
            return;
        }
        emit LoadHit(QString("数量-%1").arg(sum));
        //获取数据
        QSqlQuery* t_query = new QSqlQuery(*m_database);//临时查询，防止和GetData重复使用
        if(path=="phoneDatabase.db"){
            foreach(QString tableStr,*tableList){
                t_dataList=GetPhoneData(tableStr,addition);
                dataList->append(*t_dataList);
                delete t_dataList;
            }
        }
        else if(path=="recordDatabase.db"){
            foreach(QString tableStr,*tableList){
                t_recordList=GetRecordData(tableStr,addition);
                recordList->append(*t_recordList);
                delete t_recordList;
            }
        }
        else if(path=="errorDatabase.db"){
            foreach(QString tableStr,*tableList){
                t_errorList=GetErrorData(tableStr,addition);
                errorList->append(*t_errorList);
                delete t_errorList;
            }
        }
        else if(path=="wageDatabase.db"){
            foreach(QString tableStr,*tableList){
                t_wageList=GetWageData(tableStr,addition);
                wageList->append(*t_wageList);
                delete t_wageList;
            }
        }
        delete tableList;
        delete t_query;
    }
    else{//单表查询
        //打开并检测插入
        if(!OpenTable(path,table)){
            emit SendMessageHit("数据库-表单连接错误 "+path+table);
            return;
        }
        //获取长度
        long long sum=0;
        sum=GetLength(table,addition);
        if(sum!=-1){
            emit LoadHit(QString("数量-%1").arg(sum));

            if(path=="phoneDatabase.db")
                dataList = GetPhoneData(table,addition);
            else if(path=="recordDatabase.db")
                recordList = GetRecordData(table,addition);
            else if(path=="errorDatabase.db")
                errorList = GetErrorData(table,addition);
            else if(path=="wageDatabase.db")
                wageList = GetWageData(table,addition);
        }
    }
    CloseDatabase();//关闭
    //检测是否有数据
    if(path=="phoneDatabase.db"){
        if(dataList->size()==0){
            dataList->clear();
            delete dataList;
            emit SendPhoneData(NULL);
            emit SendMessageHit("未找到记录");
        }
        else
            emit SendPhoneData(dataList);
    }
    else if(path=="recordDatabase.db"){
        if(recordList->size()==0){
            recordList->clear();
            delete recordList;
            emit SendRecordData(NULL);
            emit SendMessageHit("未找到记录");
        }
        else
            emit SendRecordData(recordList);
    }
    else if(path=="errorDatabase.db"){
        if(errorList->size()==0){
            errorList->clear();
            delete errorList;
            emit SendErrorData(NULL);
            emit SendMessageHit("未找到记录");
        }
        else
            emit SendErrorData(errorList);
    }
    else if(path=="wageDatabase.db"){
        if(wageList->size()==0){
            wageList->clear();
            delete wageList;
            emit SendWageData(NULL);
            emit SendMessageHit("未找到记录");
        }
        else
            emit SendWageData(wageList);
    }
}
//获取数据：员工（员工的Err和Rec直接获取）
void Data::GetStaffDatabase(QString addition1,QString addition2){
    //初始化
    QList<StaffDataList*>* dataList = new QList<StaffDataList*>();
    StaffDataList* data;
    QString table;
    QString path="staffDatabase.db";
    //总表检查
    table="☺ALL☺";
    OpenDatabase(path);
    m_query = new QSqlQuery(*m_database);
    //获取所有表名
    if(!m_query->exec(QString("select * from %1%2").arg(table).arg(addition1))){
        emit LoadHit("数据库-查询所有表错误");
        return;
    }
    while (m_query->next()){
        data = new StaffDataList();
        data->ID=m_query->value("ID").toString();
        data->name=m_query->value("name").toString();
        data->inDate=m_query->value("inDate").toString();
        data->addition=m_query->value("addition").toString();
        dataList->append(data);
    }
    //获取长度
    long long sum=0;
    foreach(StaffDataList* data,(*dataList)){
        sum+=GetLength(data->ID+data->name);
    }
    emit LoadHit(QString("数量-%1").arg(sum));
    //获取数据
    foreach(StaffDataList* data,(*dataList)){
        data->errorList=GetStaffError("☺"+data->ID+"☺"+data->name+"☺ERR☺",addition2);
        data->recList=GetStaffRec("☺"+data->ID+"☺"+data->name+"☺REC☺",addition2);
    }
    CloseDatabase();//关闭
    //检测是否有数据
    if(dataList->size()==0){
        emit SendStaffData(NULL);
        emit SendMessageHit("未找到记录");
    }
    else
        emit SendStaffData(dataList);
}
//获取数据：固定资产
void Data::GetAssetsDatabase(QString addition){
    //初始化
    QList<AssetsData*>* dataList = new QList<AssetsData*>();
    AssetsData* data;
    QString table;
    QString path="countDatabase.db";
    //表检查
    table="Assets";
    OpenTable(path,table);
    //获取长度
    long long sum=0;
    m_query->exec(QString("select count(*) from %1%2").arg(table).arg(addition));
    if(m_query->next())
        sum=m_query->value(0).toLongLong();
    emit LoadHit(QString("数量-%1").arg(sum));
    //获取数据
    if(!m_query->exec(QString("select * from %1%2").arg(table).arg(addition))){
        emit LoadHit("数据库-查询所有表错误");
        return;
    }
    while (m_query->next()){
        data = new AssetsData;
        data->name=m_query->value("name").toString();
        data->count=m_query->value("count").toInt();
        data->price=m_query->value("price").toDouble();
        data->addition=m_query->value("addition").toString();
        dataList->append(data);
        emit LoadHit(QString("过程-查找固定资产"));
    }
    CloseDatabase();//关闭
    //检测是否有数据
    if(dataList->size()==0){
        emit SendAssetsData(NULL);
        emit SendMessageHit("未找到记录");
    }
    else
        emit SendAssetsData(dataList);
}
//获取数据：配件
void Data::GetToolDatabase(QString addition){
    //初始化
    QList<ToolData*>* dataList = new QList<ToolData*>();
    ToolData* data;
    QString table;
    QString path="countDatabase.db";
    //表检查
    table="Tool";
    OpenTable(path,table);
    //获取长度
    long long sum=0;
    m_query->exec(QString("select count(*) from %1%2").arg(table).arg(addition));
    if(m_query->next())
        sum=m_query->value(0).toLongLong();
    emit LoadHit(QString("数量-%1").arg(sum));
    //获取数据
    if(!m_query->exec(QString("select * from %1%2").arg(table).arg(addition))){
        emit LoadHit("数据库-查询所有表错误");
        return;
    }
    while (m_query->next()){
        data = new ToolData;
        data->name=m_query->value("name").toString();
        data->count=m_query->value("count").toInt();
        data->buyingPrice=m_query->value("buyingPrice").toFloat();
        data->salingPrice=m_query->value("salingPrice").toFloat();
        data->unit=m_query->value("unit").toString();
        dataList->append(data);
        emit LoadHit(QString("过程-查找配件"));
    }
    CloseDatabase();//关闭
    //检测是否有数据
    if(dataList->size()==0){
        emit SendToolData(NULL);
        emit SendMessageHit("未找到记录");
    }
    else
        emit SendToolData(dataList);
}
//获取多个数据：品牌、型号、配件
QStringList* Data::GetString(const QString& path,const QString& details,bool isOnly){//获取-一个数据库的全部字符串
    QStringList *tableList=new QStringList();
    QStringList *strlist=new QStringList();
    if(path=="staffDatabase.db"){
        tableList->append("☺ALL☺");
        this->OpenTable(path,tableList->first());
    }
    else if(path=="countDatabase.db"){
        tableList->append("Tool");
        this->OpenTable(path,tableList->first());
    }
    else{
        //数据库-连接数据库
        if(!OpenDatabase(path)){
            qFatal("failed to connect.") ;
            emit InitError(m_database->databaseName(),"数据库-表单连接错误");
            qDebug()<<"数据库-表单连接错误";
            return NULL;
        }
        m_query = new QSqlQuery(*m_database);
        //获取所有表名
        if(!m_query->exec(QString("select name from sqlite_master where type='table' order by name"))){
            qDebug()<<"数据库-查询所有表错误";
            return NULL;
        }
        while (m_query->next()) {
            tableList->append(m_query->value(0).toString());
        }
    }
    //获取数据
    if(path=="staffDatabase.db" && details=="IDname"){
        QString str;
        QStringList t_list;
        //获取ID
        str=QString("SELECT DISTINCT %2 FROM %1").arg("☺ALL☺").arg("ID");

        if(!m_query->exec(str)){
            return NULL;
        }
        else
        while (m_query->next()) {
            QString str=m_query->value(0).toString();
            if(!t_list.contains(str,Qt::CaseSensitive))
                t_list.append(str);
        }
        //获取name
        for(int i=0;i<t_list.count();i++){
            str=QString("SELECT name FROM %1 WHERE %2='%3'").arg("☺ALL☺").arg("ID").arg(t_list.at(i));
            if(!m_query->exec(str)){
                return NULL;
            }
            while (m_query->next())
                strlist->append(t_list.at(i)+m_query->value(0).toString());
        }
    }
    else{
        foreach(QString tableStr,*tableList){
            QString str;
            if(isOnly)
                str=QString("SELECT DISTINCT %2 FROM %1").arg(tableStr).arg(details);
            else
                str=QString("SELECT %2 FROM %1").arg(tableStr).arg(details);
            if(!m_query->exec(str)){
                qDebug()<<QString("数据库-查询%1表所有项错误").arg(tableStr);
                return NULL;
            }
            else{
                while (m_query->next()) {
                    QString str=m_query->value(0).toString();
                    if(!strlist->contains(str,Qt::CaseSensitive) || !isOnly)
                        strlist->append(str);
                }
            }
        }
    }
    CloseDatabase();//关闭
    delete tableList;
    return strlist;
}

//--添加--
bool Data::InsertDatabase(PhoneData *phoneData){//添加单个手机
    int year=GetTimeData("year");
    QString table;
    if(phoneData->inTime=="")
        table="第"+QString::number(year)+"年";
    else
        table="第"+phoneData->inTime.left(4)+"年";
    //打开并检测插入
    if(!OpenTable("phoneDatabase.db",table))
        return false;
    //插入
    if(!InsertPhoneData(table, phoneData))
        return false;
    //关闭
    CloseDatabase();
    return true;
}

bool Data::InsertDatabase(PhoneData* phoneData,int n){//添加多个相同手机
    //计算当前时间
    int year=GetTimeData("year"),month=GetTimeData("month"),day=GetTimeData("day");
    int hour=GetTimeData("hour"),minute=GetTimeData("minute"),second=GetTimeData("second");

    if(phoneData->inTime=="")//自动进货时间
        phoneData->inTime=QString("%1%2%3%4%5%6").arg(year).arg(month, 2, 10, QChar('0')).arg(day, 2, 10, QChar('0')).arg(hour, 2, 10, QChar('0')).arg(minute, 2, 10, QChar('0')).arg(second, 2, 10, QChar('0'));

    QString table="第"+phoneData->inTime.left(4)+"年";

    //打开并检测插入
    if(!OpenTable("phoneDatabase.db",table))
        return false;
    //插入
    for(int i=1;i<=n;i++){
        if(!InsertPhoneData(table, phoneData))
            return false;
        phoneData->IMEI=phoneData->IMEI.left(8)+QString("%1").arg(phoneData->IMEI.mid(8,6).toLongLong()+1,6,10,QChar('0'))+phoneData->IMEI.right(1);//自动串码
    }
    //关闭
    if(!CloseDatabase())
        return false;
    return true;
}

bool Data::InsertDatabase(ToolData *tool){//添加工具
    QString table="Tool";
    //打开并检测插入
    if(!OpenTable("countDatabase.db",table))
        return false;
    if(SearchData(table,"name",tool->name)){
        SendMessageHit("已存在该配件");
        return false;
    }
    //插入
    if(!InsertToolData(table, tool))
        return false;
    //关闭
    CloseDatabase();
    return true;
}

bool Data::InsertDatabase(PhoneRecordList *record){//添加销售记录
    QString table;
    //---员工记录---
    //初始化
    StaffRec *staffRec=new StaffRec();
    staffRec->date=record->outTime.left(8);
    staffRec->price=record->price;
    staffRec->gather=record->gather;
    staffRec->ID=record->ID;
    //打开并检测插入
    //插入记录
    if(!OpenDatabase("staffDatabase.db"))
        return false;
    m_query = new QSqlQuery(*m_database);
    if(record->shopAssistant==record->cashier){
        staffRec->work="营业员+收银员";
        table=record->shopAssistant;
        table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺REC☺";
        if(!InsertStaffData(table, staffRec))
            return false;
    }
    else{
        staffRec->work="营业员";
        table=record->shopAssistant;
        table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺REC☺";
        if(!InsertStaffData(table, staffRec))
            return false;
        staffRec->work="收银员";
        table=record->cashier;
        table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺REC☺";
        if(!InsertStaffData(table, staffRec))
            return false;
    }
    CloseDatabase();//关闭
    //---手机库存---
    QStringList strList,idList;
    strList<<"outTime"<<"shopAssistant"<<"cashier";
    foreach(PhoneRecord* data,(*record->dataList))
        if(data->IMEI!="")
            idList<<data->IMEI;
    PhoneData *phone=new PhoneData;
    phone->outTime=record->outTime;
    phone->shopAssistant=record->shopAssistant;
    phone->cashier=record->cashier;
    Update("phoneDatabase.db","IMEI",idList,strList,phone);
    delete phone;
    //---配件库存---
    QString name;
    int count;
    //数据库-连接数据库
    if(!OpenTable("countDatabase.db","Tool")){
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        return false;
    }
    m_query = new QSqlQuery(*m_database);
    //修改数据
    foreach(PhoneRecord* data,(*record->dataList)){
        name=data->phoneModels;
        if(SearchData("Tool","name",name)){
            //查找
            count=0;
            m_query->prepare(QString("select count from Tool where name='%1'").arg(name));
            if(!m_query->exec()){
                qDebug()<<"查找失败！"<<m_query->lastError();
                return false;
            }
            if(m_query->next()){
                count=m_query->value(0).toInt();
            }
            if(count==0)
                continue;
            //更新
            m_query->prepare(QString("update Tool set count=? where name='%1'").arg(name));
            m_query->addBindValue(count-data->count);
            if(!m_query->exec()){
                qDebug()<<"修更新失败！"<<m_query->lastError();
                return false;
            }
        }
    }
    CloseDatabase();//关闭
    //---销售记录---
    int year=GetTimeData("year");
    table="第"+QString::number(year)+"年";
    //打开并检测插入
    if(!OpenTable("recordDatabase.db",table))
        return false;
    //插入记录
    if(!InsertRecordData(table, record))
        return false;
    CloseDatabase();//关闭
    return true;
}

bool Data::InsertDatabase(StaffDataList *staff){//添加员工
    QString table="☺ALL☺";
    //---新员工---
    //打开并检测插入
    if(!OpenTable("staffDatabase.db",table))
        return false;
    //插入记录
    if(!InsertStaffData(table, staff))
        return false;
    CloseDatabase();//关闭
    //---初始化---
    //初始化
    if(!OpenTable("staffDatabase.db","☺"+staff->ID+"☺"+staff->name+"☺REC☺"))
        return false;
    CloseDatabase();
    if(!OpenTable("staffDatabase.db","☺"+staff->ID+"☺"+staff->name+"☺ERR☺"))
        return false;
    CloseDatabase();
    return true;
}

bool Data::InsertDatabase(ErrorData *error){//添加分摊费用
    //---分摊费用---
    int year=GetTimeData("year");
    QString table="第"+QString::number(year)+"年";
    //打开并检测插入
    if(!OpenTable("errorDatabase.db",table))
        return false;
    //插入记录
    if(!InsertErrorData(table, error))
        return false;
    CloseDatabase();//关闭
    //---员工记录---
    //初始化
    StaffError *staffError=new StaffError();
    staffError->ID=error->ID;
    staffError->name=error->name;
    staffError->date=error->date;
    staffError->reason=error->reason;
    staffError->unSale="00";
    //打开并检测插入
    if(!OpenDatabase("staffDatabase.db"))
        return false;
    m_query = new QSqlQuery(*m_database);
    for(int i=0;i<error->staffIDname.count();i++){
        staffError->price=-error->staffPrice.at(i);
        table=error->staffIDname.at(i);
        table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺ERR☺";
        if(!InsertStaffData(table, staffError))
            return false;
    }
    CloseDatabase();//关闭
    return true;
}

bool Data::InsertDatabase(WageData *wageData,QList<StaffRec*> *recList,QList<StaffError*> *errList){//添加工资
    //---添加工资---
    QString table="第"+wageData->startDate.left(4)+"年";
    //打开并检测插入
    if(!OpenTable("wageDatabase.db",table)){
        CloseDatabase();
        return false;
    }
    //查看是否重复
    if(!m_query->exec(QString("select * from %1 where ((startDate>='%2' and startDate<='%3') or (endDate>='%2' and endDate<='%3')) and IDname='%4'").arg(table).arg(wageData->startDate).arg(wageData->endDate).arg(wageData->IDname)))
    {
        qDebug()<<"查找失败"<<m_query->lastError();
        CloseDatabase();
        return false;
    }
    if(m_query->next()){
        emit SendMessageHit("该时间段和已结算时间重复");
        CloseDatabase();
        return false;
    }
    //插入
    if(!InsertWageData(table, wageData)){
        CloseDatabase();
        return false;
    }
    CloseDatabase();
    //---员工记录---
    //打开并检测插入
    if(!OpenDatabase("staffDatabase.db"))
        return false;
    m_query = new QSqlQuery(*m_database);
    table=wageData->IDname;
    table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺";
    //rec
    if(recList!=NULL && recList->count()>0)
    foreach(StaffRec *rec,(*recList)){
        m_query->prepare(QString("update %1 set %4=? where %2='%3' and (%4='00' or %4='21' or %4='10')")
                         .arg(table+"REC☺").arg("ID").arg(rec->ID).arg("unSale"));
        m_query->addBindValue(rec->unSale);
        if(!m_query->exec()){
            qDebug()<<m_query->lastQuery()<<m_query->lastError()<<"unSale"<<rec->unSale;
            return false;
        }
    }
    //err
    if(errList!=NULL && errList->count()>0)
    foreach(StaffError *err,(*errList)){
        m_query->prepare(QString("update %1 set %4=? where %2='%3' and (%4='00' or %4='21' or %4='10')")
                         .arg(table+"ERR☺").arg("ID").arg(err->ID).arg("unSale"));
        m_query->addBindValue(err->unSale);
        if(!m_query->exec()){
            qDebug()<<m_query->lastQuery()<<m_query->lastError()<<"unSale"<<err->unSale;
            return false;
        }
    }
    CloseDatabase();//关闭
    //---标记销售记录---
    if(!OpenDatabase("recordDatabase.db"))
        return false;
    m_query = new QSqlQuery(*m_database);
    for(int index=0;index<recList->count();index++){
        table="第"+recList->at(index)->ID.left(4)+"年";
        //检查
        m_query->prepare(QString("create table %1 (ID nvarchar(12), outTime nvarchar(15), shopAssistant nvarchar(30), cashier nvarchar(30), price float, gather float, unSale nvarchar(16),"
                                 "IMEI nvarchar, phoneModels nvarchar, buyingPrice nvarchar, salingPrice nvarchar, unit nvarchar, count nvarchar)").arg(table));
        m_query->exec();
        //标记
        m_query->prepare(QString("update %1 set unSale=? where ID='%2'").arg(table).arg(recList->at(index)->ID));
        m_query->addBindValue(recList->at(index)->unSale);
        if(!m_query->exec()){
            qDebug()<<"修改失败！"<<m_query->lastError();
            return false;
        }
    }
    CloseDatabase();//关闭
    //---标记分摊记录---
    if(!OpenDatabase("errorDatabase.db"))
        return false;
    m_query = new QSqlQuery(*m_database);
    for(int index=0;index<errList->count();index++){
        table="第"+errList->at(index)->ID.left(4)+"年";
        m_query->prepare(QString("update %1 set unSale=? where ID='%2'").arg(table).arg(errList->at(index)->ID));
        if(errList->at(index)->unSale.length()==2)
            m_query->addBindValue(errList->at(index)->unSale);
        else
            m_query->addBindValue("21");
        if(!m_query->exec()){
            qDebug()<<"修改失败！"<<m_query->lastError();
            return false;
        }
    }
    return true;
}

bool Data::InsertDatabase(AssetsData *asset){//添加固定资产
    QString table="Assets";
    //打开并检测插入
    if(!OpenTable("countDatabase.db",table))
        return false;
    //插入记录
    if(!InsertAssetData(table, asset))
        return false;
    CloseDatabase();//关闭
    return true;
}

//--删除--
bool Data::DeleteDatabase(QString path, QString IDName, QStringList IDList){//其他删除
    //数据库-连接数据库
    if(!OpenDatabase(path)){
        qFatal("failed to connect.") ;
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        qDebug()<<"数据库-表单连接错误";
        return false;
    }
    m_query = new QSqlQuery(*m_database);
    QStringList* tableList=new QStringList();
    //获取所有表名
    if(!m_query->exec(QString("select name from sqlite_master where type='table' order by name"))){
        qDebug()<<"数据库-查询所有表错误";
        return false;
    }
    while (m_query->next()) {
        tableList->append(m_query->value(0).toString());
    }
    //获取长度
    emit LoadHit(QString("数量-%1").arg(IDList.count()));
    //删除数据
    foreach(QString tableStr,*tableList){
        foreach(QString ID,IDList){
            if(!DeleteData(tableStr,IDName,ID)){
                emit LoadHit(QString("删除失败!"));
                delete tableList;
                return false;
            }
        }
    }
    delete tableList;
    //关闭
    CloseDatabase();
    emit LoadHit(QString("删除成功!"));
    return true;
}

bool Data::DeleteDatabase(QString path,QString table,QString IDName, QStringList IDList){//固定资产、配件
    //数据库-连接数据库
    if(!OpenDatabase(path)){
        qFatal("failed to connect.") ;
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        qDebug()<<"数据库-表单连接错误";
        return false;
    }
    m_query = new QSqlQuery(*m_database);
    //获取长度
    emit LoadHit(QString("数量-%1").arg(IDList.count()));
    //删除数据
    foreach(QString ID,IDList){
        if(!DeleteData(table,IDName,ID)){
            emit LoadHit(QString("删除失败!"));
            return false;
        }
    }
    //关闭
    CloseDatabase();
    emit LoadHit(QString("删除成功!"));
    return true;
}

bool Data::DeleteDatabase(QString path,QStringList IDList,QStringList strList){//工资    
    QStringList IDname;

    if(!OpenTable("staffDatabase.db","☺ALL☺")){
        qFatal("failed to connect.") ;
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        qDebug()<<"数据库-表单连接错误";
        return false;
    }
    foreach(QString ID,IDList)
        IDname.append(ID.left(4)+GetStaffName(ID.left(4)));
    //关闭
    delete m_query;
    m_database->close();
    delete m_database;
    //---删工资---
    //数据库-连接数据库
    if(!OpenDatabase(path)){
        qFatal("failed to connect.") ;
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        qDebug()<<"数据库-表单连接错误";
        return false;
    }
    m_query = new QSqlQuery(*m_database);
    QStringList* tableList=new QStringList();
    //获取所有表名
    if(!m_query->exec(QString("select name from sqlite_master where type='table' order by name"))){
        qDebug()<<"数据库-查询所有表错误";
        return false;
    }
    while (m_query->next()) {
        tableList->append(m_query->value(0).toString());
    }
    //获取长度
    emit LoadHit(QString("数量-%1").arg(IDList.count()));
    //删除数据
    foreach(QString table,*tableList){
        int i=0;
        foreach(QString ID,IDList){
            if(strList.at(i).left(4)!=table.mid(1,4)){
                i++;
                continue;
            }
            if(!m_query->exec(QString("delete from %1 where IDname='%2' and startDate='%3'").arg(table).arg(IDname.at(i)).arg(strList.at(i)))){
                emit LoadHit(QString("删除失败!"));
                return false;
            }
            emit LoadHit(QString("过程-删除"));
            i++;
        }
    }
    delete tableList;
    CloseDatabase();//关闭
    emit LoadHit(QString("删除成功!"));
    return true;
}

bool Data::DeleteDatabase(QStringList IDnameList){//删除员工
    //数据库-连接数据库
    if(!OpenDatabase("staffDatabase.db")){
        qFatal("failed to connect.") ;
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        qDebug()<<"数据库-表单连接错误";
        return false;
    }
    m_query = new QSqlQuery(*m_database);
    //获取长度
    emit LoadHit(QString("数量-%1").arg(IDnameList.count()*2));
    //删除表
    foreach(QString str,IDnameList){
        if(str.left(4)=="0000"){
            if(!m_query->exec(QString("delete FROM %1").arg("☺"+str.left(4)+"☺"+str.right(str.length()-4)+"☺ERR☺")))
                emit LoadHit(QString("删除失败!"));
            if(!m_query->exec(QString("delete FROM %1").arg("☺"+str.left(4)+"☺"+str.right(str.length()-4)+"☺REC☺")))
                emit LoadHit(QString("删除失败!"));

        }
        else{
            if(!m_query->exec(QString("drop table %1").arg("☺"+str.left(4)+"☺"+str.right(str.length()-4)+"☺ERR☺")))
                emit LoadHit(QString("删除失败!"));
            if(!m_query->exec(QString("drop table %1").arg("☺"+str.left(4)+"☺"+str.right(str.length()-4)+"☺REC☺")))
                emit LoadHit(QString("删除失败!"));
        }

        emit LoadHit(QString("过程-删除"));
    }
    //删除数据
    foreach(QString str,IDnameList){
        if(str.left(4)!="0000" && !DeleteData("☺ALL☺","ID",str.left(4))){
            emit LoadHit(QString("删除失败!"));
            return false;
        }
    }
    //关闭
    CloseDatabase();
    emit LoadHit(QString("删除成功!"));
    return true;
}

//--更新--
bool Data::Update(QString path, QString IDName,QStringList IDList ,QStringList strList, PhoneData* data){//手机
    QStringList* tableList=new QStringList();
    QString sql;
    //数据库-连接数据库
    if(!OpenDatabase(path)){
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        return false;
    }
    m_query = new QSqlQuery(*m_database);
    //获取所有表名
    if(!m_query->exec(QString("select name from sqlite_master where type='table' order by name"))){
        emit LoadHit(QString("数据库-查询所有表错误"));
        return false;
    }
    while (m_query->next()) {
        tableList->append(m_query->value(0).toString());
    }

    //获取长度
    emit LoadHit(QString("数量-%1").arg(IDList.count()));
    //获取数据
    foreach(QString tableStr,*tableList){
        foreach(QString ID,IDList){
            if(SearchData(tableStr,"IMEI",ID)){
                foreach(QString strName,strList){
                    sql=QString("update %1 set %4=? where %2='%3'")
                            .arg(tableStr).arg(IDName).arg(ID).arg(strName);
                    m_query->prepare(sql);
                    m_query->addBindValue(data->GetData("phoneWrite",strName));
                    if(!m_query->exec()){
                        qDebug()<<"修改失败！"<<m_query->lastError();
                        return false;
                    }
                }
                if(strList.contains("inTime") && tableStr!=("第"+data->GetData("phoneWrite","inTime").left(4)+"年")){//手机改变入库年份
                    QString newTable="第"+data->GetData("phoneWrite","inTime").left(4)+"年";
                    //创建表
                    m_query->prepare(QString("create table %1 (IMEI nvarchar(15), phoneBrand nvarchar(30), phoneModels nvarchar(30), buyingPrice float, salingPrice float,"
                                             "addition nvarchar, inTime nvarchar(14), outTime nvarchar(14), shopAssistant nvarchar(30), cashier nvarchar(30))").arg(newTable));
                    if(m_query->exec())
                        qDebug()<<"数据库-新表单创建成功";
                    else{
                        //数据库-检查创建表-表已存在-检测预处理
                        m_query->exec(QString("select * from %1").arg(newTable));
                        QSqlRecord rec = m_query->record();
                        //数据库-检查创建表-表已存在-检测字段数
                        if(rec.count()!=m_col[path]){
                            emit DataError("Database-Table-Complete-Count",newTable);
                            qDebug()<<"数据库-新表单不完整:字段数 "<<path<<"-"<<newTable<<":"<<rec.count();
                            return false;
                        }
                        //数据库-检查创建表-表已存在-检测字段名称
                        QStringList selectList=m_map[path];
                        foreach(QString str,selectList){
                            if(rec.fieldName(selectList.indexOf(str)) != str){
                                emit DataError("Database-Table-Complete-Name",newTable);
                                qDebug()<<QString("数据库-新表单不完整:%1 %2").arg(newTable).arg(str);
                                return false;
                            }
                        }
                        qDebug()<<"数据库-新表单完整";
                    }
                    //复制到新表
                    sql=QString("INSERT INTO %2 SELECT * FROM %1 WHERE %3='%4'")
                            .arg(tableStr).arg("第"+data->GetData("phoneWrite","inTime").left(4)+"年").arg(IDName).arg(ID);
                    if(!m_query->exec(sql)){
                        qDebug()<<"修改移动-复制失败！"<<m_query->lastError();
                        return false;
                    }
                    //删除
                    if(!DeleteData(tableStr,IDName,ID)){
                        qDebug()<<"修改移动-删除失败！";
                        return false;
                    }
                }
                emit LoadHit(QString("过程-修改数据"));
            }
            else
                break;
        }
    }
    delete tableList;
    CloseDatabase();//关闭
    return true;
}

bool Data::Update(QString name,QStringList strList,ToolData* data){//配件
    QString sql;
    QString table="Tool";
    //数据库-连接数据库
    if(!OpenTable("countDatabase.db","Tool")){
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        return false;
    }
    //修改数据
    if(SearchData(table,"name",name)){
        for(int i=strList.count()-1;i>=0;i--){
            QString strName=strList.at(i);
            sql=QString("update %1 set %4=? where %2='%3'")
                    .arg(table).arg("name").arg(name).arg(strName);
            m_query->prepare(sql);
            m_query->addBindValue(data->GetData("write",strName));
            if(!m_query->exec()){
                qDebug()<<"修改失败！"<<m_query->lastError();
                return false;
            }
        }
    }
    CloseDatabase();//关闭
    return true;
}

bool Data::Update(QString ID,QString name,QStringList strList,StaffDataList* data){//员工
    QString sql;
    QString table="☺ALL☺";
    //数据库-连接数据库
    if(!OpenDatabase("staffDatabase.db")){
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        return false;
    }
    m_query = new QSqlQuery(*m_database);
    //修改数据
    if(SearchData(table,"ID",ID)){
        if(strList.contains("name") || strList.contains("ID")){
            if(!m_query->exec(QString("ALTER TABLE '%1' RENAME TO '%2'").arg("☺"+ID+"☺"+name+"☺REC☺").arg("☺"+data->ID+"☺"+data->name+"☺REC☺"))){
                qDebug()<<"修改失败！"<<m_query->lastError();
                return false;
            }
            if(!m_query->exec(QString("ALTER TABLE '%1' RENAME TO '%2'").arg("☺"+ID+"☺"+name+"☺ERR☺").arg("☺"+data->ID+"☺"+data->name+"☺ERR☺"))){
                qDebug()<<"修改失败！"<<m_query->lastError();
                return false;
            }
        }
        foreach(QString strName,strList){
            sql=QString("update %1 set %4=? where %2='%3'")
                    .arg(table).arg("ID").arg(ID).arg(strName);
            m_query->prepare(sql);
            m_query->addBindValue(data->GetData("write",strName));
            if(!m_query->exec()){
                qDebug()<<"修改失败！"<<m_query->lastError();
                return false;
            }
        }
    }
    CloseDatabase();//关闭
    return true;
}

bool Data::Update(QString name,QStringList strList,AssetsData* data){//资产
    QString sql;
    QString table="Assets";
    //数据库-连接数据库
    if(!OpenTable("countDatabase.db",table)){
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        return false;
    }
    //修改数据
    if(SearchData(table,"name",name)){
        foreach(QString strName,strList){
            sql=QString("update %1 set %4=? where %2='%3'")
                    .arg(table).arg("name").arg(name).arg(strName);
            m_query->prepare(sql);
            m_query->addBindValue(data->GetData("write",strName));
            if(!m_query->exec()){
                qDebug()<<"修改失败！"<<m_query->lastError();
                return false;
            }
        }
    }
    CloseDatabase();//关闭
    return true;
}

bool Data::Update(QString ID,QStringList strList,ErrorData* data){//分摊
    QString sql;
    QString path= "errorDatabase.db";
    QString table="第"+ID.left(4)+"年";
    QString nextID;
    ErrorData* lastList=NULL;
    if(strList.contains("date")){
        nextID=GetNextID(path,"ID",data->date.left(8));
        if(nextID=="RangeUp"){
            SendMessageHit("当天超出分摊数量");
            return false;
        }
    }
    else
        nextID="";
    //数据库-连接数据库
    if(!OpenTable(path,table)){
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        return false;
    }
    //修改数据
    if(SearchData(table,"ID",ID)){
        foreach(QString strName,strList){
            if(strName=="list"){
                lastList=this->GetErrorData(table,QString(" where ID='%1'").arg(ID))->first();
            }
            sql=QString("update %1 set %4=? where %2='%3'")
                    .arg(table).arg("ID").arg(ID).arg(strName);
            m_query->prepare(sql);
            m_query->addBindValue(data->GetData("write",strName));
            if(!m_query->exec()){
                qDebug()<<"修改失败！"<<m_query->lastError();
                return false;
            }
        }
        //改ID
        if(strList.contains("date")){
            sql=QString("update %1 set %4=? where %2='%3'")
                    .arg(table).arg("ID").arg(ID).arg("ID");
            m_query->prepare(sql);
            m_query->addBindValue(nextID);
            if(!m_query->exec()){
                qDebug()<<"修改失败！"<<m_query->lastError();
                return false;
            }
        }
        //移动
        if(strList.contains("date") && "第"+data->date.left(4)+"年"!=table){
            QString newTable="第"+data->date.left(4)+"年";
            //创建表
            m_query->prepare(QString("create table %1 (ID nvarchar(10), name nvarchar(50), date nvarchar(10), reason nvarchar(1000), price double, list nvarchar)").arg(newTable));
            if(m_query->exec())
                qDebug()<<"数据库-新表单创建成功";
            else{
                //数据库-检查创建表-表已存在-检测预处理
                m_query->exec(QString("select * from %1").arg(newTable));
                QSqlRecord rec = m_query->record();
                //数据库-检查创建表-表已存在-检测字段数
                if(rec.count()!=m_col[path]){
                    emit DataError("Database-Table-Complete-Count",newTable);
                    qDebug()<<"数据库-新表单不完整:字段数 "<<path<<"-"<<newTable<<":"<<rec.count();
                    return false;
                }
                //数据库-检查创建表-表已存在-检测字段名称
                QStringList selectList=m_map[path];
                foreach(QString str,selectList){
                    if(rec.fieldName(selectList.indexOf(str)) != str){
                        emit DataError("Database-Table-Complete-Name",newTable);
                        qDebug()<<QString("数据库-新表单不完整:%1 %2").arg(newTable).arg(str);
                        return false;
                    }
                }
                qDebug()<<"数据库-表单完整";
            }
            //复制到新表
            sql=QString("INSERT INTO %2 SELECT * FROM %1 WHERE %3='%4'")
                    .arg(table).arg(newTable).arg("ID").arg(nextID);
            if(!m_query->exec(sql)){
                qDebug()<<"修改移动-复制失败！"<<m_query->lastError();
                return false;
            }
            //删除
            if(!DeleteData(table,"ID",nextID)){
                qDebug()<<"修改移动-删除失败！";
                return false;
            }
        }
    }
    CloseDatabase();//关闭
    //---员工---    
    //打开并检测插入
    if(!OpenDatabase("staffDatabase.db"))
        return false;
    m_query = new QSqlQuery(*m_database);
    //列表
    if(strList.contains("list")){
        //删除
        for(int i=0;i<lastList->staffIDname.count();i++){
            table=data->staffIDname.at(i);
            table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺ERR☺";
            if(!DeleteData(table, "ID", ID))
                return false;
        }
        delete lastList;
        //插入
        StaffError *staffErr=new StaffError();
        staffErr->ID=data->ID;
        staffErr->name=data->name;
        staffErr->date=data->date;
        staffErr->reason=data->reason;
        staffErr->unSale=data->unSale;
        for(int i=0;i<data->staffIDname.count();i++){
            staffErr->price=-data->staffPrice.at(i);
            table=data->staffIDname.at(i);
            table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺ERR☺";
            if(!InsertStaffData(table, staffErr))
                return false;
        }
    }
    //日期
    if(strList.contains("date") || strList.contains("ID")){
        //插入
        foreach(QString IDname,data->staffIDname){
            if(SearchData("☺ALL☺","ID",IDname.left(4))){
                table=IDname;
                table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺ERR☺";
                if(!SearchData(table,"ID",ID))
                    continue;
                //修改
                if(strList.contains("date")){
                    m_query->prepare(QString("update %1 set date=? where ID='%2'").arg(table).arg(ID));
                    m_query->addBindValue(data->date);
                    if(!m_query->exec()){
                        qDebug()<<"修改失败！"<<m_query->lastError();
                        return false;
                    }
                    m_query->prepare(QString("update %1 set ID=? where ID='%2'").arg(table).arg(ID));
                    m_query->addBindValue(nextID);
                    if(!m_query->exec()){
                        qDebug()<<"修改失败！"<<m_query->lastError();
                        return false;
                    }
                }
            }
        }
    }
    CloseDatabase();//关闭
    data->ID=nextID;
    return true;
}

//--查找--
//查找是否存在值
bool Data::SearchDataBase(QString path,QString details,QString value){
    //获取
    QStringList* tableList=new QStringList();
    //数据库-连接数据库
    if(!OpenDatabase(path)){
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        qDebug()<<"数据库-表单连接错误";
        return false;
    }
    m_query = new QSqlQuery(*m_database);
    //获取所有表名
    if(path=="staffDatabase.db")
        tableList->append("☺ALL☺");
    else{
        if(!m_query->exec(QString("select name from sqlite_master where type='table' order by name"))){
            qDebug()<<"数据库-查询所有表错误";
            return false;
        }
        while (m_query->next()) {
            tableList->append(m_query->value(0).toString());
        }
    }
    //寻找数据
    foreach(QString tableStr,*tableList){
        switch(SearchData(tableStr,details,value)){
            case -1:
                delete tableList;
                CloseDatabase();
                return false;
            case 1:
                delete tableList;
                CloseDatabase();
                return true;
        }
    }
    delete tableList;
    CloseDatabase();
    return false;
}
//查找下一个ID
QString Data::GetNextID(QString path,QString idname){
    //打开并检测插入
    QString table="☺ALL☺";
    if(!OpenTable(path,table))
        return "";
    //获取
    QString next_id = 0;
    if(!m_query->exec(QString("select max(%2) from %1").arg(table).arg(idname))){
        qDebug()<<m_query->lastError();
        return "";
    }
    m_query->next();
    next_id = m_query->value(0).toString();
    //关闭
    if(!CloseDatabase())
        return "";
    if(next_id==0)
        return "0001";
    if(next_id=="9999")
        return "RangeUp";
    return QString("%1").arg(next_id.toInt()+1,4,10,QChar('0'));
}
QString Data::GetNextID(QString path,QString idname,QString date){
    //打开并检测插入
    QString table="第"+date.left(4)+"年";
    if(!OpenTable(path,table))
        return "";
    //获取
    date=date.left(8);
    QString next_id = "";
    if(!m_query->exec(QString("select max(%2) from %1 where %2 like '%3%%'").arg(table).arg(idname).arg(date)))
    {
        qDebug()<<m_query->lastError()<<m_query->lastQuery();
        return "error";
    }
    if(m_query->next())
        next_id = m_query->value(0).toString();
    //关闭
    if(!CloseDatabase())
        return "";
    if(next_id=="")
        return date+"0001";
    if(next_id.right(4)=="9999")
        return "RangeUp";
    return QString::number(next_id.toLongLong()+1);
}
//查找剩余IMEI
long long Data::GetLastIMEI(QString path,QString vaule){
    QString str;
    QStringList* tableList=new QStringList();
    long long min=999999;
    //数据库-连接数据库
    if(!OpenDatabase(path)){
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        qDebug()<<"数据库-表单连接错误";
        return -1;
    }
    m_query = new QSqlQuery(*m_database);
    //获取所有表名
    if(!m_query->exec(QString("select name from sqlite_master where type='table' order by name"))){
        qDebug()<<"数据库-查询所有表错误";
        return -1;
    }
    while (m_query->next())
        tableList->append(m_query->value(0).toString());
    //寻找数据
    foreach(QString tableStr,*tableList){
        if(!m_query->exec(QString("select IMEI from %1 where IMEI like '%2%%' and IMEI>%3").arg(tableStr).arg(vaule.left(8)).arg(vaule))){
            qDebug()<<"数据库-查询错误";
            return -1;
        }
        while (m_query->next()){
            str=m_query->value(0).toString();
            if(str.mid(8,6).toLongLong()<min)
                min=str.mid(8,6).toLongLong();
        }
    }
    delete tableList;
    CloseDatabase();
    if(min==999999)
        min=vaule.mid(8,6).toLongLong();
    return min;
}

//退货
bool Data::UnSaleBasedata(PhoneRecordList *record){
    QString table;
    QString str;
    QStringList IDList;
    QStringList strList;
    //---退手机---
    foreach(PhoneRecord* dd,(*record->dataList))
        if(dd->IMEI!="")
            IDList.append(dd->IMEI);
    if(IDList.count()>0){
        strList<<"outTime"<<"shopAssistant"<<"cashier";
        PhoneData *phone=new PhoneData();
        phone->outTime="";
        phone->shopAssistant="";
        phone->cashier="";
        Update("phoneDatabase.db", "IMEI", IDList,strList,phone);
        delete phone;
    }
    //---退配件---
    QString name;
    int count;
    //数据库-连接数据库
    if(!OpenTable("countDatabase.db",table)){
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        return false;
    }
    //修改数据
    foreach(PhoneRecord* data,(*record->dataList)){
        if(data->IMEI!="")
            continue;
        name=data->phoneModels;
        if(SearchData("Tool","name",name)){
            //查找
            count=0;
            m_query->prepare(QString("select count from Tool where name='%1'").arg(name));
            if(!m_query->exec()){
                qDebug()<<"查找失败！"<<m_query->lastError();
                return false;
            }
            if(m_query->next()){
                count=m_query->value(0).toInt();
            }
            //更新
            m_query->prepare(QString("update Tool set count=? where name='%1'").arg(name));
            m_query->addBindValue(count+data->count);
            if(!m_query->exec()){
                qDebug()<<"修更新失败！"<<m_query->lastError();
                return false;
            }
        }
    }
    CloseDatabase();//关闭
    //---查看是否已结算工资---
    //00为未退货未结算，10为已退货未结算，01未退货已结算，11为已退货已结算，21退货时已结算,date是退货时已结算并处理完
    if(record->unSale=="00")
        str="10";
    else if(record->unSale=="01")
        str="21";
    //---退业绩（员工记录、影响工资）---
    //打开并检测插入
    if(!OpenDatabase("staffDatabase.db"))
        return false;
    m_query = new QSqlQuery(*m_database);
    if(record->shopAssistant==record->cashier && SearchData("☺ALL☺","ID",record->shopAssistant.left(4))){
        table=record->shopAssistant;
        table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺REC☺";
        m_query->prepare(QString("update %1 set unSale=? where ID='%2'").arg(table).arg(record->ID));
        m_query->addBindValue(str);
        if(!m_query->exec()){
            qDebug()<<"修改失败！"<<m_query->lastError()<<m_query->lastQuery();
            return false;
        }
    }
    else{
        if(SearchData("☺ALL☺","ID",record->shopAssistant.left(4))){
            table=record->shopAssistant;
            table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺REC☺";
            m_query->prepare(QString("update %1 set unSale=? where ID='%2'").arg(table).arg(record->ID));
            m_query->addBindValue(str);
            if(!m_query->exec()){
                qDebug()<<"修改失败！"<<m_query->lastError();
                return false;
            }
        }
        if(SearchData("☺ALL☺","ID",record->shopAssistant.left(4))){
            table=record->cashier;
            table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺REC☺";
            m_query->prepare(QString("update %1 set unSale=? where ID='%2'").arg(table).arg(record->ID));
            m_query->addBindValue(str);
            if(!m_query->exec()){
                qDebug()<<"修改失败！"<<m_query->lastError();
                return false;
            }
        }
    }
    CloseDatabase();//关闭
    //---退销售记录---
    table="第"+record->outTime.left(4)+"年";
    if(!OpenTable("recordDatabase.db",table))
        return false;
    m_query->prepare(QString("update %1 set unSale=? where ID='%2'").arg(table).arg(record->ID));
    m_query->addBindValue(str);
    if(!m_query->exec()){
        qDebug()<<"修改失败！"<<m_query->lastError();
        return false;
    }
    CloseDatabase();//关闭
    record->unSale=str;
    return true;
}

bool Data::UnSaleBasedata(ErrorData *error){
    QString table;
    QString str;
    //---退业绩（员工记录、影响工资）---
    //打开并检测插入
    if(!OpenDatabase("staffDatabase.db"))
        return false;
    m_query = new QSqlQuery(*m_database);
    foreach(QString IDname,error->staffIDname){
        if(SearchData("☺ALL☺","ID",IDname.left(4))){
            table=IDname;
            table="☺"+table.left(4)+"☺"+GetStaffName(table.left(4))+"☺ERR☺";
            //查看员工情况
            if(!m_query->exec(QString("select unSale from %1 where ID='%2'").arg(table).arg(error->ID))){
                qDebug()<<"修改失败！"<<m_query->lastError();
                return false;
            }
            if(m_query->next())
                str=m_query->value("unSale").toString();
            if(str=="00")
                str="10";
            else if(str=="01")
                str="21";
            //修改
            m_query->prepare(QString("update %1 set unSale=? where ID='%2'").arg(table).arg(error->ID));
            m_query->addBindValue(str);
            if(!m_query->exec()){
                qDebug()<<"修改失败！"<<m_query->lastError();
                return false;
            }
        }
    }
    CloseDatabase();//关闭
    //---查看是否已结算工资---
    //00为未退货未结算，10为已退货未结算，01未退货已结算，11为已退货已结算，21退货时已结算,date是退货时已结算并处理完
    if(error->unSale=="00")
        str="10";
    else if(error->unSale=="01")
        str="21";
    else
        str=error->unSale;
    error->unSale=str;
    //---退分摊记录---
    table="第"+error->ID.left(4)+"年";
    if(!OpenTable("errorDatabase.db",table))
        return false;
    m_query->prepare(QString("update %1 set unSale=? where ID='%2'").arg(table).arg(error->ID));
    m_query->addBindValue(str);
    if(!m_query->exec()){
        qDebug()<<"修改失败！"<<m_query->lastError();
        return false;
    }
    CloseDatabase();//关闭
    return true;
}
//登录
QString Data::GetStaffNameBasedata(QString ID){
    QString re;
    //数据库-连接数据库
    if(!OpenDatabase("staffDatabase.db")){
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        return "-1";
    }
    m_query = new QSqlQuery(*m_database);
    //修改数据
    re=this->GetStaffName(ID);
    CloseDatabase();//关闭
    return re;
}

bool Data::EditPassword(QString ID,QString password){//修改密码
    QString sql;
    QString table="☺ALL☺";
    //数据库-连接数据库
    if(!OpenDatabase("staffDatabase.db")){
        emit InitError(m_database->databaseName(),"数据库-表单连接错误");
        return false;
    }
    m_query = new QSqlQuery(*m_database);
    //修改数据
    if(SearchData(table,"ID",ID)){
        sql=QString("update %1 set %4=? where %2='%3'")
                .arg(table).arg("ID").arg(ID).arg("password");
        m_query->prepare(sql);
        m_query->addBindValue(password);
        if(!m_query->exec()){
            qDebug()<<"修改失败！"<<m_query->lastError();
            return false;
        }
    }
    CloseDatabase();//关闭
    return true;
}

short Data::Login(QString ID,QString password){//登录
    //-2系统错误
    //-1不存在用户名
    //0密码不正确
    //1密码正确
    //初始化
    QString t_ID="",t_password="";
    QString table;
    QString path="staffDatabase.db";
    //总表检查
    table="☺ALL☺";
    OpenTable(path,table);
    //获取所有表名
    if(!m_query->exec(QString("select * from %1 where ID='%2'").arg(table).arg(ID))){
        emit LoadHit("数据库-查询所有表错误");
        return -2;
    }
    if(m_query->next()){
        t_ID=m_query->value("ID").toString();
        t_password=m_query->value("password").toString();
    }
    CloseDatabase();//关闭
    if(t_ID=="")
        return -1;
    if(t_password!=password)
        return 0;
    return 1;
}

