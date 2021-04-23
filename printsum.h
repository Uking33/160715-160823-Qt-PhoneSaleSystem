#ifndef PRINTFSUM_H
#define PRINTFSUM_H

//窗口
#include <QObject>
#include <QtPrintSupport>
//外部数据
class MainWindow;
class PhoneData;
class PhoneRecordList;

class PrintSum:public QObject
{
    Q_OBJECT
public:

    PrintSum(MainWindow *parent);
    ~PrintSum();
    void doPrintPreview(PhoneRecordList *record);
    void doPrint(PhoneRecordList *record);
    void Watch(PhoneRecordList *record);
    QPrinter *m_printer;

private:

    MainWindow *m_parent;
    QList<PhoneData *> *m_list;
    PhoneRecordList *m_record;
    QString numberTurnUp(QString c);
private slots:
    void printToPage(QPrinter *printer); //打印
signals:
    void PrintBegin();
};

#endif // PRINTFSUM_H
