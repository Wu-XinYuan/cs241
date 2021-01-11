#ifndef TIMECHOICEDDIALOG_H
#define TIMECHOICEDDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class TimeChoicedDialog;
}

class TimeChoicedDialog : public QDialog
{
    Q_OBJECT

public slots:
    void receiveData(QStandardItemModel*);

signals:
    void sendData(QStandardItemModel*);
    void sendTime(int*);


public:
    explicit TimeChoicedDialog(QWidget *parent = nullptr);
    ~TimeChoicedDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::TimeChoicedDialog *ui;
    QStandardItemModel *data;
    int time[2];
};

#endif // TIMECHOICEDDIALOG_H
