#define QDBG qDebug()<<__FILE__<<__FUNCTION__<<__LINE__
#include <QDebug>
#include <QApplication>
class MainWindow;
class GlobalApplication : public QApplication
{
public:
     GlobalApplication(int&argc,char **argv);
     ~GlobalApplication();

     bool notify(QObject*, QEvent *);
     void setWindowInstance(MainWindow *wnd);
private:
     MainWindow *widget;
};
