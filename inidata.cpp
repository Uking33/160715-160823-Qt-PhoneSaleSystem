#include "inidata.h"
#include "QSettings"

IniData::IniData()
{
    Loading();
}

IniData::~IniData()
{

}

void IniData::Loading(){
    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
    //---账单---
    //读
    m_recordTitle=configIni->value("/record/title").toString();
    m_recordAddress=configIni->value("/record/address").toString();
    m_recordPhone=configIni->value("/record/phone").toString();
    m_recordAddition=configIni->value("/record/addition").toString();
    if(m_recordTitle==""||m_recordAddress==""||m_recordPhone==""||m_recordAddition==""){
        //写
        m_recordTitle="珠海市川龙通信统一销售票据";
        m_recordAddress="地址：珠海市香洲区明珠南路华业大夏一楼11号铺（川龙通讯）";
        m_recordPhone="电话:0756-8657999";
        m_recordAddition="感谢您购买本公司的移动电话，我们将保障您的权益如下：\n"
                         "一：1、于正常使用状况下，因制造上的瑕疵所产生的故障，本公司及生产厂家提供自购买日起（壹年）内免费保修（液晶显示屏除外）"
                         "。如属质量问题十五天内包换同型号新机。（注：必须送厂家维修检测中心并故障属实且外壳无刮花、无进水，外包装、配件完好）。\n"
                         "   2、手机配件（充电器、电池等）保用三个月（须电池无刮花、保修标完整)，赠品不保。\n"
                         "二、唯有下列情况，则不在免费检测保修范围之内：\n"
                         "   1、由于正常磨损引起和周期性检查，维护，维修和更换部件。2、凡购买以后造成的外壳表面（如天线、镜片、前壳、后壳、翻盖"
                         "等）及液晶显示屏的损坏，属人为原因不予保修。3、由于坠落、挤压、浸水、受潮等或其它人为造成的损坏。4、由于网络参数的变更"
                         "而引起的软件升级，水灾、火灾、雷等等自然灾害造成的损坏。5、电池过量充电造成的损坏或不按照产品说明书中的维护说明引起的故"
                         "障。6、由非授权人员拆机维修。7、产品上标明的型号、编号和制造号已经更改、保修标签损毁、删除、移动或不可辩认。\n"
                         "三、新购手机包换期或免费保修期内出现问题必须送到本公司或各生产厂商指定的维修检测中心，送检或送修时间一般需七至十五个工"
                         "作日（注：节假日除外），敬请谅解，如自行送到其它地方检测维修，出现争议本公司概不负责。\n"
                         "*保修范围：非人为损坏免费保修一年，人为损坏如需换零件只收取成本费。*此价格不含税价*";
        configIni->setValue("/record/title",m_recordTitle);
        configIni->setValue("/record/address",m_recordAddress);
        configIni->setValue("/record/phone",m_recordPhone);
        configIni->setValue("/record/addition",m_recordAddition);
    }
    //---登录---
    //读
    m_loginName=configIni->value("/login/name").toString();
    if(m_loginName=="")//写
        configIni->setValue("/login/name","");
}

void IniData::recordSave(){
    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
    configIni->setValue("/record/title",m_recordTitle);
    configIni->setValue("/record/address",m_recordAddress);
    configIni->setValue("/record/phone",m_recordPhone);
    configIni->setValue("/record/addition",m_recordAddition);
}

void IniData::loginSave(){
    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
    configIni->setValue("/login/name",m_loginName);
}
