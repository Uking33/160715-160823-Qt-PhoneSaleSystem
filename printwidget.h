#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

//窗口
#include <QObject>
#include <QtPrintSupport>
class MainWindow;

class PrintWidget:public QObject
{
    Q_OBJECT
public:
    MainWindow *m_parent;
    QTableWidget *m_table;
    QTableWidget *m_preTable;
    QString m_title;

    PrintWidget(MainWindow *parent, QTableWidget *table, QString title);
    ~PrintWidget();

    void Start();
private:
    QPrintPreviewDialog *m_preview;

private slots:
    void doPrintPreview();
    void printPreview(QPrinter *printer);
    void printTableWidget(const QTableWidget* view,QString stitile,QPrinter *printer); //printFlag =1,打印表格

};

#endif // PRINTWIDGET_H
