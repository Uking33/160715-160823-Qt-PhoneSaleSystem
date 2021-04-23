//窗口
#include "printsum.h"
#include <QPrintDialog>
//外部数据
#include "mainwindow.h"
#include "type.h"
//数据
#include <QList>
#include "inidata.h"
//辅助
#include <QDate>

PrintSum::PrintSum(MainWindow *parent){
    m_parent=parent;
    m_printer=new QPrinter();
    m_printer->setPageSizeMM(QSizeF(210,148.5));
    m_printer->setOrientation(QPrinter::Portrait);
    m_printer->setPageMargins(0.6,0.5,0.3,2.5,QPrinter::Millimeter);
}

PrintSum::~PrintSum(){
    delete m_printer;
}

// 打印预览
void PrintSum::doPrintPreview(PhoneRecordList *record){
    m_record=record;
    // 创建打印预览对话框
    QPrintPreviewDialog preview(m_printer);
    QToolBar *toolBar=(QToolBar *)preview.children().at(6)->children().at(2);
    toolBar->setEnabled(false);
    // 当要生成预览页面时，发射paintRequested()信号
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
                  this,SLOT(printToPage(QPrinter*)));
    connect(&preview, &QPrintPreviewDialog::accepted,[=](){
        emit PrintBegin();
    });
    preview.exec();
    disconnect(&preview, SIGNAL(paintRequested(QPrinter*)),
                  this,SLOT(printToPage(QPrinter*)));
}

void PrintSum::doPrint(PhoneRecordList *record){
    m_record=record;
    printToPage(m_printer);
    emit PrintBegin();
}

void PrintSum::Watch(PhoneRecordList *record){
    m_record=record;
    // 创建打印预览对话框
    QPrintPreviewDialog preview(m_printer,m_parent);
    preview.setMinimumSize(1100,650);
    preview.move(m_parent->x(),m_parent->y());
    QToolBar *toolBar=(QToolBar *)preview.children().at(6)->children().at(2);
    toolBar->setEnabled(false);
    // 当要生成预览页面时，发射paintRequested()信号
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
                  this,SLOT(printToPage(QPrinter*)));
    preview.exec();
    disconnect(&preview, SIGNAL(paintRequested(QPrinter*)),
                  this,SLOT(printToPage(QPrinter*)));
}

void PrintSum::printToPage(QPrinter *printer) //打印
{
    //数值初始化
    int i,j;
    int count;
    count=4;
    QList<int> rowPix;
    rowPix<<29<<17<<23<<29;
    for(i=0;i<count;i++)
        rowPix<<25;
    rowPix<<28<<23<<19<<17<< 190 <<19;
    QList<int> colPix;
    colPix<<0<<160<<249<<73<<67<<75<<95<<20;
    QList<QRect> rect;
    int tc,tr=0;
    for(i=0;i<count+10;i++){
        tc=0;
        for(j=0;j<8;j++){
            rect.append(QRect(tc,tr,colPix.at(j),rowPix.at(i)));
            tc+=colPix.at(j);
        }
        tr+=rowPix.at(i);
    }

    QPainter painter(printer);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);//反锯齿
    painter.begin(printer);
    //横线
    for(i=3;i<=count+5;i++){
        if (i == 3){
            painter.drawLine( (rect.at(i*8+1).topLeft()) , (rect.at(i*8+6).topRight()) );
        }
        painter.drawLine( (rect.at(i*8+1).bottomLeft()) , (rect.at(i*8+6).bottomRight()) );
    }
    //竖线
    for(i=1;i<=6;i++){
        if (i==1)
            painter.drawLine( (rect.at(3*8+i).topLeft()) , (rect.at((5+count)*8+i).bottomLeft()) );
        if (i==1 || i==6)
            painter.drawLine( (rect.at(3*8+i).topRight()) , (rect.at((5+count)*8+i).bottomRight()) );
        if (i==2 || i==3)
            painter.drawLine( (rect.at(3*8+i).topRight()) , (rect.at((3+count)*8+i).bottomRight()) );
        if (i==4 || i==5)
            painter.drawLine( (rect.at(3*8+i).topRight()) , (rect.at((4+count)*8+i).bottomRight()) );
    }
    //文字-标题
    QFont font;
    font.setFamily("宋体");
    font.setPointSize(18);
    font.setBold(true);
    font.setUnderline(true);
    painter.setFont(font);
    painter.drawText(QRectF(QPointF(rect.at(1).left(),rect.at(1).top()),QPointF(rect.at(6).right(),rect.at(6).bottom()))
            ,Qt::AlignCenter,m_parent->m_ini->m_recordTitle);
    //文字-头
    font.setPointSize(11);
    font.setBold(false);
    font.setUnderline(false);
    painter.setFont(font);
    if(m_record->outTime=="")
        m_record->outTime=QDate::currentDate().toString("yyyyMMdd");
    painter.drawText(QRectF(QPointF(rect.at(8+1).left(),rect.at(8+1).top()),QPointF(rect.at(8+3).right(),rect.at(8+3).bottom()))
            ,Qt::AlignLeft|Qt::AlignVCenter,QString("     开单日期:%1年%2月%3日").arg(m_record->outTime.left(4)).arg(m_record->outTime.mid(4,2)).arg(m_record->outTime.mid(6,2)));
    painter.drawText(QRectF(QPointF(rect.at(8+4).left(),rect.at(8+4).top()),QPointF(rect.at(8+6).right(),rect.at(8+6).bottom()))
            ,Qt::AlignLeft|Qt::AlignVCenter,QString("  单号:%1").arg(m_record->ID));
    painter.drawText(QRectF(QPointF(rect.at(8*2+1).left(),rect.at(8*2+1).top()),QPointF(rect.at(8*2+3).right(),rect.at(8*2+3).bottom()))
            ,Qt::AlignLeft|Qt::AlignVCenter,"     "+m_parent->m_ini->m_recordAddress);
    painter.drawText(QRectF(QPointF(rect.at(8*2+4).left(),rect.at(8*2+4).top()),QPointF(rect.at(8*2+6).right(),rect.at(8*2+6).bottom()))
            ,Qt::AlignLeft|Qt::AlignVCenter,"  "+m_parent->m_ini->m_recordPhone);
    //文字-表头
    QStringList strList;
    strList<<"品   名   规   格"<<"IMEI（号码）"<<"单位"<<"数量"<<"单价(¥)"<<"金额(¥)";
    for(i=1,j=0;i<=6;i++,j++)
        painter.drawText(rect.at(8*3+i),Qt::AlignCenter,strList.at(j));
    //文字-表
    strList.clear();
    strList<<"phoneModels"<<"IMEI"<<"unit"<<"1"<<"salingPrice"<<"salingPrice";
    for(i=0;i<m_record->dataList->count();i++){
        painter.drawText(rect.at((4+i)*8+1),Qt::AlignCenter,m_record->dataList->at(i)->phoneModels);
        painter.drawText(rect.at((4+i)*8+2),Qt::AlignCenter,m_record->dataList->at(i)->IMEI);
        painter.drawText(rect.at((4+i)*8+3),Qt::AlignCenter,m_record->dataList->at(i)->unit);
        painter.drawText(rect.at((4+i)*8+4),Qt::AlignCenter,QString::number(m_record->dataList->at(i)->count));
        painter.drawText(rect.at((4+i)*8+5),Qt::AlignCenter,QString::number(m_record->dataList->at(i)->salingPrice,'f',2));
        painter.drawText(rect.at((4+i)*8+6),Qt::AlignCenter,QString::number(m_record->dataList->at(i)->salingPrice*m_record->dataList->at(i)->count,'f',2));
    }
    //文字-表尾
    painter.drawText(rect.at(8*(4+count)+1),Qt::AlignRight|Qt::AlignVCenter,"合      计     大写:");
    painter.drawText(rect.at(8*(4+count)+5),Qt::AlignRight|Qt::AlignVCenter,"小写：");
    double sum=m_record->Sum();
    QString ch[7];
    QString str=QString::number(sum,'d',2);
    ch[0]=numberTurnUp(str.right(1));
    ch[1]=numberTurnUp(str.right(2).left(1));

    if(str.length()>=4) ch[2]=numberTurnUp(str.right(4).left(1));
    if(str.length()>=5) ch[3]=numberTurnUp(str.right(5).left(1));
    if(str.length()>=6) ch[4]=numberTurnUp(str.right(6).left(1));
    if(str.length()>=7) ch[5]=numberTurnUp(str.right(7).left(1));
    if(str.length()>=8) ch[6]=numberTurnUp(str.right(8).left(1));

    painter.drawText(rect.at(8*(4+count)+6),Qt::AlignCenter,str);
    painter.drawText(QRectF(QPointF(rect.at(8*(4+count)+2).left(),rect.at(8*(4+count)+2).top()),QPointF(rect.at(8*(4+count)+4).right(),rect.at(8*(4+count)+4).bottom()))
                     ,Qt::AlignCenter,QString(" %1万 %2仟 %3佰 %4拾 %5元 %6角 %7分").arg(ch[6]).arg(ch[5]).arg(ch[4]).arg(ch[3]).arg(ch[2]).arg(ch[1]).arg(ch[0]));
    painter.drawText(rect.at(8*(5+count)+1),Qt::AlignCenter,"备        注");

    //文字-签名
    str=m_record->shopAssistant;
    painter.drawText(rect.at(8*(6+count)+1),Qt::AlignRight|Qt::AlignVCenter,"营 业 员:");
    painter.drawText(rect.at(8*(6+count)+2),Qt::AlignLeft|Qt::AlignVCenter,str.remove(0,4));
    painter.drawText(QRectF(QPointF(rect.at(8*(6+count)+3).left(),rect.at(8*(6+count)+3).top()),QPointF(rect.at(8*(6+count)+4).right(),rect.at(8*(6+count)+4).bottom()))
                     ,Qt::AlignLeft|Qt::AlignVCenter,"客 户 签 名：");

    str=m_record->cashier;
    painter.drawText(rect.at(8*(7+count)+1),Qt::AlignRight|Qt::AlignVCenter,"收 银 员:");
    painter.drawText(rect.at(8*(7+count)+2),Qt::AlignLeft|Qt::AlignVCenter,str.remove(0,4));
    painter.drawText(QRectF(QPointF(rect.at(8*(7+count)+3).left(),rect.at(8*(7+count)+3).top()),QPointF(rect.at(8*(7+count)+4).right(),rect.at(8*(7+count)+4).bottom()))
                     ,Qt::AlignLeft|Qt::AlignVCenter,"联 系 电 话：");
    //文字-说明
    font.setPointSize(9);
    painter.setFont(font);
    strList.clear();
    strList.append(m_parent->m_ini->m_recordAddition);
    painter.drawText(QRectF(QPointF(rect.at(8*(8+count)+1).left(),rect.at(8*(8+count)+1).top()),QPointF(rect.at(8*(8+count)+6).right(),rect.at(8*(8+count)+6).bottom()))
                     ,Qt::TextWrapAnywhere,strList.at(0));
    //文字-盖章
    font.setPointSize(10);
    painter.setFont(font);
    painter.drawText(QRectF(QPointF(rect.at(8*(9+count)+3).left(),rect.at(8*(9+count)+3).top()),QPointF(rect.at(8*(9+count)+4).right(),rect.at(8*(9+count)+4).bottom()))
                     ,Qt::AlignRight|Qt::AlignVCenter,"销售单位（盖章）：");
    //文字-侧边
    font.setPointSize(9);
    painter.setFont(font);
    painter.drawText(QRectF(QPointF(rect.at(8*3+7).left(),rect.at(8*3+7).top()),QPointF(rect.at(8*(9+count)+7).right(),rect.at(8*(9+count)+7).bottom()))
                     ,Qt::AlignHCenter,"\n白\n联\n存\n根\n；\n \n \n红\n联\n顾\n客\n；\n \n \n绿\n联\n仓\n库\n ");
    painter.end();
}

QString PrintSum::numberTurnUp(QString c){
    if(c==" ")
        return " ";
    switch(c.toInt()){
        case 0:
            return "零";
        case 1:
            return "壹";
        case 2:
            return "贰";
        case 3:
            return "叁";
        case 4:
            return "肆";
        case 5:
            return "伍";
        case 6:
            return "陆";
        case 7:
            return "柒";
        case 8:
            return "捌";
        case 9:
            return "玖";
    }
    return " ";
}
