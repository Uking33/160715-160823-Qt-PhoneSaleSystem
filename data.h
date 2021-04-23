
#ifndef DATA_H
#define DATA_H

#include <QObject>
//数据类型
#include <QMap>
#include <QList>
#include "type.h"

class QSqlDatabase;
class QSqlQuery;
class QDate;
class QTime;
class QDateTime;

class Data : public QObject
{
    Q_OBJECT
public:
    Data();
    ~Data();
private:
    /*一级操作*/
    //--初始化--
    bool OpenDatabase(const QString &path);
    bool CloseDatabase();
    bool OpenTable(QString path,QString table);
    //--获取--
    QList<PhoneData*>* GetPhoneData(const QString& table, const QString &value);//获取-手机
    QList<PhoneRecordList*>* GetRecordData(const QString& table, const QString &value);//获取-销售记录
    QList<StaffRec*>* GetStaffRec(const QString& table,QString addition="");//获取-员工-销售记录
    QList<StaffError*>* GetStaffError(const QString& table, QString addition="");//获取-员工-分摊
    QString GetStaffID(QString staffName);//获取-员工-ID
    QString GetStaffName(QString staffID);//获取-员工-姓名
    QList<ErrorData*>* GetErrorData(const QString& table, const QString& value);//获取-分摊
    QList<WageData*>* GetWageData(const QString& table, const QString& value);//获取-工资
    //--插入--
    bool InsertPhoneData(const QString table, PhoneData *phoneData);
    bool InsertRecordData(const QString table,PhoneRecordList *record);
    bool InsertToolData(const QString table, ToolData *tool);
    bool InsertStaffData(const QString table,StaffDataList *staffDataList);
    bool InsertStaffData(const QString table, StaffRec *staffRec);
    bool InsertStaffData(const QString table, StaffError *staffError);
    bool InsertErrorData(const QString table,ErrorData *error);
    bool InsertWageData(const QString table,WageData *wage);
    bool InsertAssetData(const QString table,AssetsData *asset);
    //--删除--
    bool DeleteData(const QString table, const QString IDName, const QString ID);
    //--查询--
    QString GetMaxID(QString table,QString idname);
    long long GetLength(QString table, QString addition="");
    short SearchData(QString table,QString details,QString value);
public slots:
    /*---二级操作---*/
    //--初始化--
    bool InitDatabase();//检测数据库
    //--获取--
    void GetDatabase(QString path, QString table, QString addition="");//手机、记录、分摊、工资
    void GetStaffDatabase(QString addition1="", QString addition2="");//员工
    void GetAssetsDatabase(QString addition="");//固定资产
    void GetToolDatabase(QString addition="");//配件
    QStringList *GetString(const QString& path, const QString& details, bool isOnly=true);//获取-一个数据库的全部字符
    //--添加--
    bool InsertDatabase(PhoneData* phoneData);//添加单个手机
    bool InsertDatabase(PhoneData* phoneData,int n);//添加多个相同手机
    bool InsertDatabase(ToolData *tool);//添加工具
    bool InsertDatabase(PhoneRecordList *m_record);//添加单个记录
    bool InsertDatabase(StaffDataList *staff);//添加员工
    bool InsertDatabase(ErrorData *error);//添加分摊费用
    bool InsertDatabase(WageData *wageData, QList<StaffRec*> *recList, QList<StaffError*> *errList);//添加工资
    bool InsertDatabase(AssetsData *asset);//添加固定资产
    //--删除--
    bool DeleteDatabase(QString path, QString IDName, QStringList IDList);
    bool DeleteDatabase(QStringList IDnameList);//删除员工
    bool DeleteDatabase(QString path,QString table,QString IDName, QStringList IDList);//固定资产、配件
    bool DeleteDatabase(QString path,QStringList IDList,QStringList strList);//工资
    //--更新--
    bool Update(QString path, QString IDName, QStringList IDList , QStringList strList, PhoneData *data);//手机
    bool Update(QString name,QStringList strList,ToolData* data);//配件
    bool Update(QString ID, QString name, QStringList strList, StaffDataList* data);//员工
    bool Update(QString name,QStringList strList,AssetsData* data);//资产
    bool Update(QString ID, QStringList strList, ErrorData* data);//分摊
    //--查找--
    bool SearchDataBase(QString path,QString details,QString value);//查找是否存在串码
    QString GetNextID(QString path, QString idname, QString date);    
    QString GetNextID(QString path,QString idname);
    long long GetLastIMEI(QString path,QString vaule);

    //退货
    bool UnSaleBasedata(PhoneRecordList *record);    
    bool UnSaleBasedata(ErrorData *error);

    QString GetStaffNameBasedata(QString ID);
    bool EditPassword(QString ID,QString password);
    short Login(QString ID,QString password);//登录
    //当前时间
    QString GetTimeDataStr();
    int GetTimeData(QString type);

signals:
    //获取信号
    void SendPhoneData(QList<PhoneData*>*);
    void SendToolData(QList<ToolData*>*);
    void SendRecordData(QList<PhoneRecordList *>*);
    void SendStaffData(QList<StaffDataList *>*);
    void SendAssetsData(QList<AssetsData*>*);
    void SendErrorData(QList<ErrorData *>*);
    void SendWageData(QList<WageData *>*);
    //消息错误信号
    void SendMessageHit(const QString& text);
    void DataError(const QString &type,const QString &details) const;
    void InsertError(const QString &table,const QString &type) const;
    void InitError(const QString &table,const QString &type) const;

    void LoadHit(const QString);
private:
    QMap<QString,QStringList> m_map;
    QMap<QString,int> m_col;
    QSqlDatabase *m_database;
    QSqlQuery *m_query;
};

#endif // DATA_H
