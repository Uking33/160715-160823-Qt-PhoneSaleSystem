#ifndef UBUTTON_H
#define UBUTTON_H

#include <QPushButton>
#include <QObject>

class QHBoxLayout;
class QGridLayout;

class UButton:public QPushButton
{
    Q_OBJECT
public:
    UButton(const QString &text, const QString & shortcut, int width, int height);
    UButton(const QString &text, const QString & shortcut, int width, int height, QHBoxLayout *parent);
    UButton(const QString & text,const QString & shortcut, int width, int height, QGridLayout* parent, int x=0, int y=0);
    ~UButton();
};

#endif // UBUTTON_H
