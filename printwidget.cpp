#include "printwidget.h"
#include "mainwindow.h"
#include <QPrintDialog>
#include <QDebug>
#include <QTextEdit>
#include <QTableWidget>

#include <QActionGroup>
#include <QObject>

PrintWidget::PrintWidget(MainWindow *parent, QTableWidget *table, QString title)
{
    m_preTable=table;
    m_parent=parent;
    m_title = title;
}

PrintWidget::~PrintWidget()
{
    delete m_table;
}
void PrintWidget::Start(){
    //创建表格
    QStringList headerLabels;
    headerLabels<< "串码"<< "手机品牌"<< "手机型号"<< "进货价"<< "型号库存";
    m_table=new QTableWidget(0,5);
    m_table->setFixedWidth(525);
    //行标题
    m_table->setHorizontalHeaderLabels(headerLabels);
    m_table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_table->verticalHeader()->setDefaultSectionSize(25);//行高
    m_table->setColumnWidth(0,85);
    m_table->setColumnWidth(1,170);
    m_table->setColumnWidth(2,170);
    m_table->setColumnWidth(3,50);
    m_table->setColumnWidth(4,50);
    //添加表格
    int count;
    int i,j;
    QString last;
    if(m_preTable->item(0,0)!=NULL){
        count=0;
        last=m_preTable->item(0,2)->text();
    }
    for(i=0;i<m_preTable->rowCount();i++){
        if(m_preTable->item(i,0)==NULL)
            break;
        m_table->insertRow(i);
        for(j=0;j<4;j++){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            switch(j){
                case 0:
                case 1:
                case 2:
                case 3:
                    item->setText(m_preTable->item(i,j)->text());
                    break;
            }
            m_table->setItem(i,j,item);
        }
        //添加count
        if(m_table->item(i,2)->text()!=last){
            for(j=i-1;j>=i-count;j--){
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignCenter);
                item->setText(QString("%1").arg(count));
                m_table->setItem(j,4,item);
            }
            last=m_table->item(i,2)->text();
            count=1;
        }
        else
            count++;
    }
    //添加count
    for(j=i-1;j>=i-count;j--){
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(QString("%1").arg(count));
        m_table->setItem(j,4,item);
    }
    //打印
    doPrintPreview();
}

// 打印预览
void PrintWidget::doPrintPreview(){
    QPrinter printer;
    printer.setPageSize(QPrinter::A4);
    printer.setDocName(m_title);
    printer.setOrientation(QPrinter::Portrait);
    printer.setPageMargins(10,10,10,10,QPrinter::Millimeter);

    /*
    //设置打印
    QPrintDialog printDialog(&printer);
    if (printDialog.exec()){
        printPreview(&printer);
    }*/


    //预览打印
    // 创建打印预览对话框
    m_preview = new QPrintPreviewDialog(&printer, m_parent);
    QToolBar *toolBar=(QToolBar *)m_preview->children().at(6)->children().at(2);
    //满宽
    ((QToolButton *)toolBar->children().at(3))->click();
    //排版方向
    foreach(QObject *act,(m_preview->children().at(3))->children())
        ((QAction *)act)->setEnabled(false);
    //设置
    ((QToolButton *)toolBar->children().at(30))->setEnabled(false);
    // 当要生成预览页面时，发射paintRequested()信号
    connect(m_preview, SIGNAL(paintRequested(QPrinter*)),
                  this,SLOT(printPreview(QPrinter*)));
    //QToolButton * btn=(QToolButton *)m_preview->children().at(6)->children().at(2)->children().at(31);
    m_preview->exec();
}

void PrintWidget::printPreview(QPrinter *printer){
    printTableWidget(m_table,m_title,printer);
}
void PrintWidget::printTableWidget(const QTableWidget* view,QString stitile,QPrinter *printer) //printFlag =1,打印表格
{
    //----表格总大小----
    double totalWidth = 0.0;
    double totalHeight = 0.0;
    int rows = view->model()->rowCount();   //行总数
    const int cols = view->model()->columnCount(); //列总数
    for (int c = 0; c < cols; ++c)  //求出列宽
        totalWidth += view->columnWidth(c);
    for (int r = 0; r < rows; ++r) //求出行宽
        totalHeight += view->rowHeight(r);
    //qDebug() << "rows = " << rows << ", cols = " << cols << ", totalWidth = " << totalWidth << ", totalHeight = " << totalHeight << endl;


    //----绘制图----
    QPainter painter(printer);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);//反锯齿
    painter.begin(printer);
    //每页纸范围和边距
    QRect area = printer->paperRect();//纸的范围
    qreal left,top,right,bottom;//纸的坐标
    printer->getPageMargins(&left,&top,&right,&bottom,QPrinter::Point);
    //qDebug() << "left = " << left << ", right = " << right << ", top = " << top << ", bottom = " << bottom << endl;
    area.setHeight( int(area.height() - top - bottom) );
    area.setWidth( int(area.width() - left - right) );
    //每页
    QVector<int> startRow;//每页起始行数
    QVector<int> endRow;//每页最终行数
    int iCount = 0;
    double totalPageHeight=0.0;//当前页面高度
    for (int p=0; p < rows; p++){ //rows中保存了所有的行数 ， 找出每次打印的开始行和结束行
        totalPageHeight += view->rowHeight(p); //求出所有的列高度
        //页脚页眉
        int pageFooter = 50 ;  //页脚的高度
        int pageHeader = 50;   //页眉的高度
        /*if (startRow.size() == 0){
            pageFooter = 50 ;
            pageHeader = 50;
        }*/
        //最后行处理
        if ((totalPageHeight >= (area.height() - pageFooter - pageHeader)) || (p == rows -1)){  //如果目前累加列的高度大于或者等于可用页面高度 || 到达最后一行
            totalPageHeight = view->rowHeight(p);
            if (p == rows -1){//最后页
                if (p - iCount - 2 < 0)
                    startRow.push_back(0);
                else{
                    startRow.push_back(p - iCount - 1);
                }
                endRow.push_back(p);
            }
            else{//超页
                if (p - iCount - 2 < 0){
                    startRow.push_back(0);
                    endRow.push_back(p - 2);
                }
                else{
                    startRow.push_back(p - iCount - 1);
                    endRow.push_back(p - 3);
                    p--;
                }
            }
            iCount = 0;
        }
        iCount++;
    }

    //转换坐标系
    QTableWidget *printTable = new QTableWidget();
    //printTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    //printTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //printTable->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    //printTable->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    const double xscale = area.width() / totalWidth;
    const double yscale = area.height() / totalHeight;
    //const double pscale = area.height() / totalPageHeight;
    //painter.scale(xscale , yscale); // With the scaling 3 It is Printing all
    //painter.translate(area.x() + xscale, area.y() + pscale); //This is original
    painter.translate(area.x() + xscale, area.y() + yscale);
    //painter.save(); //commented
    //int y=0 ;// This is introduced for the columns
    //view->paintEvent(&event);
    //QPainter paint(this);
    //paint.setPen(Qt::red);
    //paint.drawRect(0, 0, 0, 0);
    QStyleOptionViewItem option;
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);

    int irowcount = -1;
    float rate = 1;
    for(int ipage = 0; ipage < startRow.size();ipage++)  //startRow有大，就有多少页需要打印
    {
        printTable->setColumnCount(view->columnCount() + 1);  //打印的列数为view的列数
        printTable->setRowCount(endRow[ipage] - startRow[ipage] + 2);  //设置当前打印页的行数
        for(int c = 1;c < cols + 1;c++)
        {
            printTable->setColumnWidth( c,(view->columnWidth(c - 1)) );
            QTableWidgetItem *newItem = printTable->item(0,c);
            if (!newItem)  //如果printTable的首行不存在，添加文字或者序号
            {
                if (view->horizontalHeaderItem(c - 1)) //Returns the horizontal(水平的) header item for column
                    newItem = new QTableWidgetItem(view->horizontalHeaderItem(c - 1)->text());
                else
                    newItem = new QTableWidgetItem(QString::number(c));
                printTable->setItem(0,c ,newItem);
            }
            else    //否则，直接设置文字或者序号
            {
                if (view->horizontalHeaderItem(c - 1))
                    newItem->setText(view->horizontalHeaderItem(c - 1)->text());
                else
                    newItem->setText(QString::number(c));
            }
        }
        printTable->setColumnWidth(0,0);
    /////////////////////////求出合适的比率，用来完美表格的显示
        QRect firstTable , lastTable;
        QModelIndex firstIndex = printTable->model()->index(0,0);
        QModelIndex lastIndex = printTable->model()->index(0,cols);
        firstTable = printTable->visualRect(firstIndex);
        lastTable =  printTable->visualRect(lastIndex);
        float totalTableWidth = lastTable.topRight().x() - firstTable.topLeft().x();
        float pagerWidth = printer->paperRect().width() - 60 - 60;
        //qDebug()<<printer->paperRect().width()<<pagerWidth<<totalTableWidth;
        rate = totalTableWidth/pagerWidth;
        //qDebug() << "totalTableWidth = " << totalTableWidth << ", pagerWidth = " << pagerWidth << ", rate = " << rate << endl;
    /////////////////////////求出合适的比率，用来完美表格的显示

        for(int c = 1;c < cols + 1;c++)  //重设宽度
        {
            printTable->setColumnWidth( c,int((view->columnWidth(c - 1))/rate) );
        }
        printTable->setColumnWidth(0,view->verticalHeader()->width());  //Returns the table view's vertical header.
        int iRow = 0;
        for(int ir = startRow[ipage]; ir <= endRow[ipage];ir++) //ir变量用来标识当前打印的是哪些行，设置当前打印页的首行
        {
            printTable->setRowHeight(iRow,view->rowHeight(ir));  //Sets the height of the given row to be height.
            for(int ic = 0; ic < view->columnCount(); ic++)  //设置每个单元格的内容
            {
                QTableWidgetItem *newItem = printTable->item(iRow  + 1,ic + 1);
                if (!newItem)
                {
                    if (view->item(ir,ic))
                        newItem = new QTableWidgetItem(view->item(ir,ic)->text());
                    else
                        newItem = new QTableWidgetItem("");
                    printTable->setItem(iRow + 1,ic + 1,newItem);
                }
                else
                {
                    if (view->item(ir,ic))
                        newItem->setText(view->item(ir,ic)->text());
                    else
                        newItem->setText("");
                }
            }
            iRow++;
        }
        qreal ioffy = 50;
        /*if (ipage == 0)
        {
            ioffy = 500;
            QwtPlotPrintFilter filter;
            int options = QwtPlotPrintFilter::PrintAll;
            options &= ~QwtPlotPrintFilter::PrintBackground;
            options |= QwtPlotPrintFilter::PrintFrameWithScales;
            filter.setOptions(options);
            d_plot->print(&painter,QRect(50,65,650,400),filter);
        }*/
        rows = printTable->rowCount();
        iRow = 1;
        for(int ir = startRow[ipage] ; ir <= endRow[ipage] ; ir++)  //ir变量用来标识当前打印的是哪些行，设置当前打印页的首列序号
        {
            QTableWidgetItem *newItem = printTable->item(iRow,0);
            if (!newItem)
            {
                if (view->verticalHeaderItem(ir))
                    newItem = new QTableWidgetItem(view->verticalHeaderItem(ir)->text());
                else
                    newItem = new QTableWidgetItem(QString::number(iRow));
                printTable->setItem(iRow,0,newItem);
            }
            else
            {
                if (view->verticalHeaderItem(ir))
                    newItem->setText(view->verticalHeaderItem(ir)->text());
                else
                    newItem->setText(QString::number(iRow));
            }
            iRow ++;
        }

        //线框
        QPointF offPt(20,ioffy);  //20表示边框的空格 50?
        for (int r = 0; r < rows; r++)
        {
            QRect rt1,rt2;
            QModelIndex idx1 = printTable->model()->index(r,0);
            QModelIndex idx2 = printTable->model()->index(r,cols);
            rt1 = printTable->visualRect(idx1);
            rt2 = printTable->visualRect(idx2);
            painter.drawLine( (rt1.bottomLeft() + offPt) , (rt2.bottomRight() + offPt) );
            if (r == 0)
            {
                painter.drawLine( (rt1.topLeft() + offPt) , (rt2.topRight() + offPt) );
            }
        }
        for (int c = 0; c < cols + 1; c++)
        {
            QRect rt1,rt2;
            QModelIndex idx1 = printTable->model()->index(0,c);
            QModelIndex idx2 = printTable->model()->index(rows - 1,c);
            rt1 = printTable->visualRect(idx1);
            rt2 = printTable->visualRect(idx2);
            painter.drawLine( (rt1.topLeft() + offPt) , (rt2.bottomLeft() + offPt) );
            if (c == cols)
            {
                painter.drawLine( (rt1.topRight() + offPt) , (rt2.bottomRight() + offPt) );
            }
        }
        //内容
        painter.setFont(QFont("SimSun",10));
        for (int r = 0; r < rows; r++)
        {
            irowcount ++;
            for(int c = 0; c < cols  + 1; c++)
            {
                QModelIndex idx = printTable->model()->index(r,c);
                option.rect = printTable->visualRect(idx);
                if (printTable->item(r,c))
                {
                    QRectF rt(option.rect.left() + 3,option.rect.top(),option.rect.width(),option.rect.height());
                    rt = rt.translated(offPt);
                    if (r != 0 && c == 0)
                    {
                        painter.drawText(rt,Qt::AlignCenter,QString("%1").arg(irowcount));
                    }
                    else
                    {
                        if (r == 0)
                        {
                            painter.drawText(rt,Qt::AlignCenter,printTable->item(r,c)->text());
                        }
                        else
                        {
                            painter.drawText(rt,Qt::AlignVCenter|Qt::AlignCenter,printTable->item(r,c)->text());
                        }
                    }
                }
            }
        }
        //ipage
        //设置标题
        if (ipage == 0)
        {
            QRectF rttt(area.left(),area.top(),area.width(),50);
            painter.setFont(QFont("黑体",20));
            painter.drawText(rttt,Qt::AlignCenter,stitile);
        }
        //页码
        painter.setFont(QFont("SimSun",10));
        QRectF rttt(area.left(),area.bottom() - 65,area.width(),25);
        painter.drawText(rttt,Qt::AlignCenter,QString::number(ipage + 1));
        //下一页
        if (ipage < startRow.size() - 1)
        {
            printer->newPage();
            irowcount--;
        }
    }
    painter.end();
}
