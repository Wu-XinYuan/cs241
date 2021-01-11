#ifndef GRIDSCHANGEDIALOG_H
#define GRIDSCHANGEDIALOG_H

#include <QDialog>
#include <QLayout>
#include <QRadioButton>

namespace Ui {
class gridsChangeDialog;
}

class gridsChangeDialog : public QDialog
{
    Q_OBJECT
public slots:
    void receiveGrid(int);

signals:
    void sendGrid(int);

public:
    explicit gridsChangeDialog(QWidget *parent = nullptr);
    ~gridsChangeDialog();

private slots:
    void on_buttonBox_accepted();

//    void on_pushButton_clicked();

private:
    Ui::gridsChangeDialog *ui;
    QGridLayout *radiobuttonLayout;
    QButtonGroup* buttonsGroup;
    QRadioButton* buttons[100];
    int grid;
};

#endif // GRIDSCHANGEDIALOG_H
