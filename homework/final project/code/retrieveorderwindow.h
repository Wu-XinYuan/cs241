#ifndef RETRIEVEORDERWINDOW_H
#define RETRIEVEORDERWINDOW_H

#include <QMainWindow>
#include <QStandardItem>
#include <QDateTime>
#include <QButtonGroup>

namespace Ui {
class retrieveOrderWindow;
}

class retrieveOrderWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit retrieveOrderWindow(QWidget *parent = nullptr);
    ~retrieveOrderWindow();

signals:
    void goBack();

private slots:
    void receiveData(QStandardItemModel*);
    void select();
    void setEnabled_1(bool);
    void setEnabled_2(bool);
    void setEnabled_3(bool);
    void setEnabled_4(bool);

    void on_pushButton_3_clicked();

private:
    Ui::retrieveOrderWindow *ui;
    QStandardItemModel* data;
    QStandardItemModel* dataPresent;
    QButtonGroup* buttonGroup;
};

#endif // RETRIEVEORDERWINDOW_H
