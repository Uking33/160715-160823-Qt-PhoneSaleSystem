#ifndef ULINE_H
#define ULINE_H

//窗口
#include <QHBoxLayout>
class QGridLayout;
//控件
class QLineEdit;
class QLabel;

class ULine:public QHBoxLayout{
    Q_OBJECT
public:
    ULine(const QString& text, const QString hitText="", int widthLabel=60, int heightLabel=25, int widthEdit=200, int heightEdit=25);
    ~ULine();
    //值
    void SetLabel(const QString& text);
    void SetValue(const QString& value);
    void SetValue(const int value);
    void SetValue(const float value);
    void SetValidator(QString rect);
    QString GetValue();
    void ReValue();
    //设置
    void SetReadOnly(bool isRead);
    void SetFocus();
    void SetDouble(int aft, int max, int min=0);//只能设置一次
    void SetInt(int max, int min=0);//只能设置一次
    void SetMaxLength(int count);//只能设置一次
    void SetDoubleNotype(int aft, int max, int min=0);
    void SetIntNotype(int max, int min=0);
    void SetMaxLengthNotype(int count);

    QLabel *m_label;
    QLineEdit *m_edit;
    int m_type;
signals:
    void textEdited();
};

#endif // ULINE_H
