#include "globalapplication.h"
#include <QKeyEvent>
#include "mainwindow.h"

GlobalApplication::GlobalApplication(int &argc,char **argv):
    QApplication(argc,argv)
{
    widget=NULL;
}

GlobalApplication::~GlobalApplication()
{

}

void GlobalApplication::setWindowInstance(MainWindow *wnd)
{
     widget = wnd;
}

bool GlobalApplication::notify(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        if(widget!=NULL) widget->keyPress(keyEvent);
     }
    else if(e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
        if(widget!=NULL) widget->mousePress(mouseEvent);
    }
    else if(e->type() == QEvent::MouseButtonRelease){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
        if(widget!=NULL) widget->mouseRelease(mouseEvent);
    }
    else if(e->type() == QEvent::MouseMove){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
        if(widget!=NULL) widget->mouseMove(mouseEvent);
    }
    return QApplication::notify(obj,e);
}
