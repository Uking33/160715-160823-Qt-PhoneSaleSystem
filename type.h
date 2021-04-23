
#ifndef TYPE_H
#define TYPE_H

#include <QString>
#include <QStringList>
class PhoneData{
public:
    PhoneData();
    PhoneData(QString phoneBrand, QString phoneModels, float buyingPrice, float salingPrice,
              QString addition="", QString inTime="");
    QString GetData(QString type, QString title);
    PhoneData &operator=(const PhoneData &);   //赋值运算

    //产品信息
    QString IMEI;//串码
    QString phoneBrand;//手机品牌
    QString phoneModels;//手机型号
    float buyingPrice;//进货价
    float salingPrice;//零售价
    QString addition;//备注
    //进货
    QString inTime;//进货时间
    //出货
    QString outTime;//出货时间
    QString shopAssistant;//营业员
    QString cashier;//收银员
};

class ToolData{
public:
    QString name;   //名称
    int count; //数量
    float buyingPrice;  //进货价
    float salingPrice;  //零售价
    QString unit;   //单位
    QString GetData(QString type, QString title);
    ToolData &operator=(const ToolData &);   //赋值运算
};

class PhoneRecord{
public:
    PhoneRecord();
    ~PhoneRecord();
    QString IMEI;//串码
    QString phoneModels;//手机型号
    float salingPrice;//单价
    float buyingPrice;//进货价
    QString unit;//单位
    int count;//数量
    QString GetData(QString type, QString title);
    PhoneRecord &operator=(const PhoneRecord &);   //赋值运算
};

class PhoneRecordList{
public:
    PhoneRecordList();
    ~PhoneRecordList();

    QString ID;//单号
    QString outTime;//出货时间
    QString shopAssistant;//营业员
    QString cashier;//收银员
    double price;//售价
    double gather;//毛利
    QString unSale;//00为未退货未结算，10为已退货未结算，01未退货已结算，11为已退货已结算，21退货时已结算,date是退货时已结算并处理完
    QList<PhoneRecord*>* dataList;

    double Sum();
    QString GetData(QString type, QString title);
};


class StaffError{
public:
    QString ID;//分摊编号
    QString name;//分摊名称
    QString date;//分摊时间
    QString reason;//分摊原因
    double price;//分摊费用
    QString unSale;
};
class StaffRec{
public:
    QString date;//记录时间
    QString ID;//单号
    QString work;//职务
    double price;//销售金额
    double gather;//销售毛利
    QString unSale;
};

class StaffDataList{
public:
    StaffDataList();
    ~StaffDataList();

    QString ID;//工号
    QString name;//姓名
    QString inDate;//入职时间
    QString addition;//备注

    QList<StaffRec*>* recList;
    QList<StaffError*>* errorList;

    QString GetData(QString type, QString title, QString addition1="", QString addition2="");
};

class AssetsData{
public:
    QString name;
    int count;
    double price;
    QString addition;

    QString GetData(QString type, QString title);
};

class ErrorData{
public:
    QString ID;
    QString name;
    QString reason;
    QString date;
    double price;
    QString unSale;
    QList<QString> staffIDname;
    QList<double> staffPrice;

    QString GetData(QString type, QString title);
    QStringList GetListItem();
};

class RewordData{
public:
    QString errorID;
    QString name;
    QString reason;
    QString date;
    double price;
};

class WageData{
public:
    QString IDname;
    QString startDate;
    QString endDate;
    double wage;
    double base;
    double gather;
    double per;

    QList<RewordData> rewardList;

    QString GetData(QString type, QString title);
    QStringList GetListItem();
};

class Type{
public:
    static QStringList PHONE_ALL;
    static QStringList PHONE_PART;
    static QStringList TOOL_ALL;
    static QStringList TOOL_PART;
    static QStringList RECORD;
    static QStringList RECORD_ALL;
    static QStringList RECORD_PART;
    static QStringList STAFF;
    static QStringList Assets;
    static QStringList Error;
    static QStringList Wage;
    static QStringList WageShow;
};


#endif // TYPE_H
