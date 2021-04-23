#include "ubutton.h"
#include <QHBoxLayout>
#include <QGridLayout>

UButton::UButton(const QString &text, const QString & shortcut, int width, int height)
    :QPushButton(text)
{
    setFixedSize(width,height);//设置长宽
    if(shortcut!="")
        setShortcut(QKeySequence(shortcut));//设置快捷键
    setFocusPolicy(Qt::NoFocus);
}

UButton::UButton(const QString & text,const QString & shortcut, int width, int height, QHBoxLayout* parent)
    :QPushButton(text)
{
    setFixedSize(width,height);//设置长宽
    if(shortcut!="")
        setShortcut(QKeySequence(shortcut));//设置快捷键
    if(parent!=NULL) parent->addWidget(this);//布局
    setFocusPolicy(Qt::NoFocus);
}
UButton::UButton(const QString & text,const QString & shortcut, int width, int height, QGridLayout* parent, int x, int y)
    :QPushButton(text)
{
    setFixedSize(width,height);//设置长宽
    if(shortcut!="") setShortcut(QKeySequence(shortcut));//设置快捷键
    if(parent!=NULL) parent->addWidget(this,x,y);//布局
}


UButton::~UButton()
{

}

