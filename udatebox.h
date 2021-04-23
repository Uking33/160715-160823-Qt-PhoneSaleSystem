#ifndef UDATEBOX_H
#define UDATEBOX_H

//窗口
#include <QWidget>
class QHBoxLayout;
//控件
class QLabel;
class QComboBox;
//数据
class QDate;

class UDateBox:public QWidget
{
    Q_OBJECT
public:
    UDateBox(int state, UDateBox *lastDateBox=NULL);
    ~UDateBox();

    QString GetData();
    int GetDate(const QString type);
    QString GetTable();
    short m_state;
    QDate *m_dateMax;
    QComboBox* m_comboBoxYear;
    QComboBox* m_comboBoxMonth;
    QComboBox* m_comboBoxDay;
    UDateBox *m_lastDateBox;
public slots:
    void First(bool);
    void Today(bool);
    void ToWeek(bool);
    void ToMonth(bool);
    void ToYear(bool);
    void Update(QDate *date);
    void Update(const QString& date);
    void Update();
    void ChooseMonth(const QString& month);
    void ChooseYear(const QString& year);
    void ChooseDay(const QString& day);

    void DisconnectBox();
    void ConnectBox();
signals:
    void textChanged();
private:
    QString m_year;
    QString m_month;
    QString m_day;
    QHBoxLayout* m_layout;
    QLabel* m_label1;
    QLabel* m_label2;
    QLabel* m_label3;
};

#endif // UDATEBOX_H
