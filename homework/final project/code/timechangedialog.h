#ifndef TIMECHANGEDIALOG_H
#define TIMECHANGEDIALOG_H

#include <QDialog>

namespace Ui {
class timeChangeDialog;
}

class timeChangeDialog : public QDialog
{
    Q_OBJECT
public slots:
    void receiveTime(int *);
signals:
    void sendTime(int *);
public:
    explicit timeChangeDialog(QWidget *parent = nullptr);
    ~timeChangeDialog();

private slots:
    void on_spinBox_5_valueChanged(int arg1);
    void on_spinBox_10_valueChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::timeChangeDialog *ui;
    int time[10];
};

#endif // TIMECHANGEDIALOG_H
