#include "type.h"

QStringList Type::PHONE_ALL = QList<QString>()<< "IMEI"<< "phoneBrand"<< "phoneModels"<< "buyingPrice"<< "salingPrice"
                                                   << "inTime"<< "outTime"<< "shopAssistant"<< "cashier"<<"addition";
QStringList Type::PHONE_PART = QList<QString>()<< "IMEI"<< "phoneBrand"<< "phoneModels"<< "salingPrice"
                                                    << "inTime"<<"addition";

QStringList Type::TOOL_ALL = QList<QString>()<< "name"<< "count"<< "buyingPrice"<< "salingPrice"<< "unit";

QStringList Type::TOOL_PART = QList<QString>()<< "name"<< "count"<< "salingPrice"<< "unit";

QStringList Type::RECORD_ALL = QList<QString>()<< "name"<< "IMEI"<< "buyingPrice"<< "salingPrice"<< "count"<< "unit";

QStringList Type::RECORD_PART = QList<QString>()<< "name"<< "IMEI"<< "salingPrice"<< "count"<< "unit";

QStringList Type::RECORD = QList<QString>()<<"ID"<<"outTime"<<"shopAssistant"<<"cashier"<<"price"<<"gather"<<"IMEI";

QStringList Type::STAFF = QList<QString>()<<"ID"<<"name"<<"inTime"<<"shopAssistCount"<<"cashyCount"<<"count"<<"shopAssistPrice"<<"gather"<<"addition";

QStringList Type::Assets = QList<QString>()<<"name"<<"count"<<"price"<<"allPrice"<<"addition";

QStringList Type::Error = QList<QString>()<<"name"<<"date"<<"reason"<<"price"<<"list";

QStringList Type::Wage = QList<QString>()<<"name"<< "startDate"<< "endDate"<< "wage"<< "base"<< "gather"<< "per"<< "list"<<"error";
QStringList Type::WageShow = QList<QString>()<<"name"<< "date"<< "wage"<< "base"<< "gather"<< "per"<< "allList";

PhoneData::PhoneData(){
    //产品信息
    this->IMEI="";//串码
    this->phoneBrand="";//手机品牌
    this->phoneModels="";//手机型号
    this->buyingPrice=0;//进货价
    this->salingPrice=0;//零售价
    this->addition="";//备注
    //进货
    this->inTime="";//进货时间
    //出货
    this->outTime="";//出货时间
    this->shopAssistant="";//营业员
    this->cashier="";//收银员
}

PhoneData::PhoneData(QString phoneBrand, QString phoneModels, float buyingPrice, float salingPrice, QString addition, QString inTime){
    //产品信息
    this->IMEI="";//串码
    this->phoneBrand=phoneBrand;//手机品牌
    this->phoneModels=phoneModels;//手机型号
    this->buyingPrice=buyingPrice;//进货价
    this->salingPrice=salingPrice;//零售价
    this->addition=addition;//备注
    //进货
    this->inTime=inTime;//进货时间
    //出货
    this->outTime="";//出货时间
    this->shopAssistant="";//营业员
    this->cashier="";//收银员
}

QString PhoneData::GetData(QString type, QString title){
    QString str;
    if(type=="phone"){
        switch(Type::PHONE_ALL.indexOf(title)){
            case 0:
                return this->IMEI;
            case 1:
                return this->phoneBrand;
            case 2:
                return this->phoneModels;
            case 3:
                return "¥"+QString::number(this->buyingPrice,'f',2);
            case 4:
                return "¥"+QString::number(this->salingPrice,'f',2);
            case 5:
                if(this->inTime=="")
                    return "未知";
                str=this->inTime;
                return str.left(4)+"年"+str.mid(4,2)+"月"+str.mid(6,2)+"日"+str.mid(8,2)+":"+str.mid(10,2)+":"+str.mid(12,2);
            case 6:
                if(this->outTime=="")
                    return "未出售";
                str=this->outTime;
                return str.left(4)+"年"+str.mid(4,2)+"月"+str.mid(6,2)+"日"+str.mid(8,2)+":"+str.mid(10,2)+":"+str.mid(12,2);
            case 7:
                if(this->shopAssistant=="")
                    return "";
                return this->shopAssistant;
            case 8:
                if(this->cashier=="")
                    return "";
                return this->cashier;
            case 9:
                return this->addition;
        }
    }
    if(type=="phoneWrite"){
        switch(Type::PHONE_ALL.indexOf(title)){
            case 0:
                return this->IMEI;
            case 1:
                return this->phoneBrand;
            case 2:
                return this->phoneModels;
            case 3:
                return QString::number(this->buyingPrice);
            case 4:
                return QString::number(this->salingPrice);
            case 5:
                return this->inTime;
            case 6:
                return this->outTime;
            case 7:
                return this->shopAssistant;
            case 8:
                return this->cashier;
            case 9:
                return this->addition;
        }
    }
    return "error";
}
PhoneData & PhoneData::operator=(const PhoneData & n)   //重载赋值运算符
{
    IMEI = n.IMEI;
    phoneBrand = n.phoneBrand;
    phoneModels = n.phoneModels;
    buyingPrice = n.buyingPrice;
    salingPrice = n.salingPrice;
    addition = n.addition;
    inTime = n.inTime;
    outTime = n.outTime;
    shopAssistant = n.shopAssistant;
    cashier = n.cashier;
    return *this;
}

QString ToolData::GetData(QString type, QString title){
    if(type=="show"){
        switch(Type::TOOL_ALL.indexOf(title)){
            case 0:
                return name;
            case 1:
                return QString::number(count);
            case 2:
                return "¥"+QString::number(buyingPrice,'f',2);
            case 3:
                return "¥"+QString::number(salingPrice,'f',2);
            case 4:
                return unit;
        }
    }
    if(type=="write"){
        switch(Type::TOOL_ALL.indexOf(title)){
            case 0:
                return name;
            case 1:
                return QString::number(count);
            case 2:
                return QString::number(this->buyingPrice,'f',2);
            case 3:
                return QString::number(this->salingPrice,'f',2);
            case 4:
                return unit;
        }
    }
    return "error";
}

ToolData & ToolData::operator=(const ToolData & n)   //重载赋值运算符
{
    name = n.name;
    count = n.count;
    buyingPrice = n.buyingPrice;
    salingPrice = n.salingPrice;
    unit = n.unit;
    return *this;
}

PhoneRecord::PhoneRecord(){}
PhoneRecord::~PhoneRecord(){}

PhoneRecordList::PhoneRecordList(){
    dataList = new QList<PhoneRecord*>();
}

PhoneRecordList::~PhoneRecordList(){
    foreach(PhoneRecord *data,(*dataList))
        delete data;
    delete dataList;
}

QString PhoneRecordList::GetData(QString type, QString title){
    QString str;
    if(type=="show"){
        switch(Type::RECORD.indexOf(title)){
            case 0:
                return this->ID;
            case 1://开单时间
                if(unSale!="01" && unSale!="00")
                    str="已退货";
                else{
                    str=outTime.left(4)+"年"+outTime.mid(4,2)+"月"+outTime.mid(6,2)+"日"+
                            outTime.mid(8,2)+":"+outTime.mid(10,2)+":"+outTime.mid(12,2);
                }
                return str;
            case 2:
                return this->shopAssistant;
            case 3:
                return this->cashier;
            case 4://售价
                return "¥"+QString::number(price,'d',2);
            case 5://毛利
                return "¥"+QString::number(gather,'d',2);
            case 6:
                str.clear();
                foreach(PhoneRecord *data,(*dataList)){
                    str+=',';
                    str+=data->phoneModels;
                }
                str.remove(0,1);
                return str;
        }
    }
    return "error";
}
double PhoneRecordList::Sum(){
    double sum=0;
    foreach (PhoneRecord *data, (*dataList)) {
        sum+=data->salingPrice*data->count;
    }
    return sum;
}

QString PhoneRecord::GetData(QString type, QString title){
    if(type=="show"){
        switch(Type::RECORD_ALL.indexOf(title)){
            case 0:
                return phoneModels;
            case 1:
                return IMEI;
            case 2:
                return "¥"+QString::number(buyingPrice,'f',2);
            case 3:
                return "¥"+QString::number(salingPrice,'f',2);
            case 4:
                return QString::number(count);
            case 5:
                return unit;
        }
    }
    return "error";
}

PhoneRecord & PhoneRecord::operator=(const PhoneRecord & n)   //重载赋值运算符
{
    phoneModels = n.phoneModels;
    IMEI = n.IMEI;
    buyingPrice = n.buyingPrice;
    salingPrice = n.salingPrice;
    unit = n.unit;
    count = n.count;
    return *this;
}


StaffDataList::StaffDataList(){
    recList = new QList<StaffRec*>();
    errorList = new QList<StaffError*>();
}

StaffDataList::~StaffDataList(){
    foreach(StaffRec *data,(*recList))
        delete data;
    delete recList;
    foreach(StaffError *data,(*errorList))
        delete data;
    delete errorList;
}

QString StaffDataList::GetData(QString type, QString title, QString addition1,QString addition2){
    QString str;
    long long count;
    double sum;
    if(type=="show"){
        switch(Type::STAFF.indexOf(title)){
            case 0:
                return this->ID;
            case 1:
                return this->name;
            case 2:
                str=inDate.left(4)+"年"+inDate.mid(4,2)+"月"+inDate.mid(6,2)+"日";
                return str;
            case 3://营业次数
                count=0;
                foreach(StaffRec* data,(*recList))
                    if(data->date>=addition1 && data->date<=addition2)
                        if(data->work.contains("营业员"))
                        count++;
                return QString::number(count)+"次";
            case 4://收银次数
                count=0;
                foreach(StaffRec* data,(*recList))
                    if(data->date>=addition1 && data->date<=addition2)
                        if(data->work.contains("收银员"))
                        count++;
                return QString::number(count)+"次";
            case 5://销售量
                count=0;
                foreach(StaffRec* data,(*recList))
                    if(data->date>=addition1 && data->date<=addition2)
                        if(data->work.contains("营业员"))
                        count++;
                return QString::number(count)+"单";
            case 6://售价
                sum=0;
                foreach(StaffRec* data,(*recList)){
                    if(data->date>=addition1 && data->date<=addition2 && data->work.contains("营业员") && ((data->unSale.length()==2&&data->unSale.left(1)=="0")||data->unSale.length()==16))
                        sum+=data->price;
                    if(data->unSale.length()==16 &&
                            (data->unSale.left(8)>=addition1 && data->unSale.left(8)<=addition2))
                        sum-=data->price;
                }
                return "¥"+QString::number(sum,'d',2);
            case 7://毛利
                sum=0;
                foreach(StaffRec* data,(*recList)){
                    if(data->date>=addition1 && data->date<=addition2 && data->work.contains("营业员") && ((data->unSale.length()==2&&data->unSale.left(1)=="0")||data->unSale.length()==16))
                        sum+=data->gather;
                    if(data->unSale.length()==16 &&
                            (data->unSale.left(8)>=addition1 && data->unSale.left(8)<=addition2))
                        sum-=data->gather;
                }
                return "¥"+QString::number(sum,'d',2);
            case 8:
                return this->addition;
        }
    }
    else if(type=="write"){
        switch(Type::STAFF.indexOf(title)){
            case 0:
                return this->ID;
            case 1:
                return this->name;
            case 2:
                return this->inDate;
            case 8:
                return this->addition;
        }
    }
    return "error";
}

QString ErrorData::GetData(QString type, QString title){
    QString str;
    if(type=="show"){
        switch(Type::Error.indexOf(title)){
            case 0:
                return name;
            case 1:
                if(unSale=="00" || unSale=="01")
                    str=date.left(4)+"年"+date.mid(4,2)+"月"+date.mid(6,2)+"日";
                else
                    str="已取消";
                return str;
            case 2:
                return reason;
            case 3:
                return "¥"+QString::number(price,'f',2);
            case 4:
                str.clear();
                for(int i=0;i<staffPrice.count();i++){
                    str+=staffIDname.at(i)+"-"+QString::number(staffPrice.at(i)/price*100,'d',1)+"%-"+"¥"+QString::number(staffPrice.at(i),'d',2)+" ";
                }
                if(str.length()!=0) str.remove(str.length()-1,1);
                return str;
        }
    }
    else if(type=="write"){
        switch(Type::Error.indexOf(title)){
            case 0:
                return name;
            case 1:
                return date;
            case 2:
                return reason;
            case 3:
                return QString::number(price);
            case 4:
                str.clear();
                for(int i=0;i<staffPrice.count();i++){
                    str+=staffIDname.at(i)+"-"+QString::number(staffPrice.at(i))+"☺";
                }
                if(str.length()!=0) str.remove(str.length()-1,1);
                return str;
        }
    }
    return "error";
}

QStringList ErrorData::GetListItem(){
    QStringList strList;
    if(staffIDname.length()!=0){
        strList.clear();
        for(int i=0;i<staffPrice.count();i++){
            strList.append(staffIDname.at(i)+"  "+QString::number(staffPrice.at(i)/price*100,'d',1)+"%  ¥"+QString::number(staffPrice.at(i)));
        }
        return strList;
    }
    return QStringList();
}
QString AssetsData::GetData(QString type, QString title){
    if(type=="show"){
        switch(Type::Assets.indexOf(title)){
            case 0:
                return name;
            case 1:
                return QString("%1个").arg(count);
            case 2:
                return "¥"+QString::number(price,'f',2);
            case 3:
                return "¥"+QString::number(price*count,'f',2);
            case 4:
                return addition;
        }
    }
    else if(type=="write"){
        switch(Type::Assets.indexOf(title)){
            case 0:
                return name;
            case 1:
                return QString::number(count);
            case 2:
                return QString::number(price);
            case 4:
                return addition;
        }
    }
    return "error";
}


QStringList WageData::GetListItem(){
    QStringList list;
    foreach(RewordData reward,rewardList)
        if(reward.price>0)
            list.append(reward.name+"(取消返还) ¥"+QString::number(reward.price,'f',2)+" "+reward.reason);
        else
            list.append(reward.name+" ¥"+QString::number(reward.price,'f',2)+" "+reward.reason);
    return list;
}

QString WageData::GetData(QString type, QString title){
    QString str;
    if(type=="show"){
        switch(Type::Wage.indexOf(title)){
            case 0:
                return IDname;
            case 1:
                return QString("%1-%2-%3").arg(startDate.left(4)).arg(startDate.mid(4,2)).arg(startDate.mid(6,2));
            case 2:
                return QString("%1-%2-%3").arg(endDate.left(4)).arg(endDate.mid(4,2)).arg(endDate.mid(6,2));
            case 3:
                return "¥"+QString::number(wage,'f',2);
            case 4:
                return "¥"+QString::number(base,'f',2);
            case 5:
                return "¥"+QString::number(gather,'f',2);
            case 6:
                return QString::number(per,'f',1)+"%";
            case 7:
                str.clear();
                foreach(RewordData reward,rewardList)
                    if(reward.errorID=="")
                        str.append(reward.name+":¥"+QString::number(reward.price,'f',2)+"\n       ");
                return str.remove(str.length()-8,8);
            case 8:
                str.clear();
                foreach(RewordData reward,rewardList)
                    if(reward.errorID!="")
                        str.append(reward.name+":¥"+QString::number(reward.price,'f',2)+"\n       ");
                return str.remove(str.length()-8,8);
        }
        if(title=="date"){
            return QString("%1-%2-%3").arg(startDate.left(4)).arg(startDate.mid(4,2)).arg(startDate.mid(6,2))+
            "至\n"+QString("%1-%2-%3").arg(endDate.left(4)).arg(endDate.mid(4,2)).arg(endDate.mid(6,2));
        }
        else if(title=="allList"){
            str.clear();
            foreach(RewordData reward,rewardList)
                str.append(reward.name+":¥"+QString::number(reward.price,'f',2)+" ");
            return str;
        }
    }
    return "error";
}
