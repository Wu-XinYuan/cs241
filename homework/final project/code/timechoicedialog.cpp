#include "timechoicedialog.h"
#include "ui_timechoicedialog.h"
#include <QString>

TimeChoicedDialog::TimeChoicedDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeChoicedDialog)
{
    ui->setupUi(this);
    ui->comboBox_1->addItem(QString::number(2016));
    ui->comboBox_4->addItem(QString::number(2016));
    ui->comboBox_2->addItem(QString::number(11));
    ui->comboBox_5->addItem(QString::number(11));
    for (int i=1; i<=15; ++i) {
        ui->comboBox_3->addItem(QString::number(i),i);
        ui->comboBox_6->addItem(QString::number(i),i);
    }
    ui->comboBox_6->setCurrentIndex(14);
    ui->comboBox_7->addItem("ChengDu");
}

TimeChoicedDialog::~TimeChoicedDialog()
{
    delete ui;
}

void TimeChoicedDialog::receiveData(QStandardItemModel* d){
    data=d;
}

void TimeChoicedDialog::on_buttonBox_accepted()
{
    time[0]=ui->comboBox_3->currentIndex();
    time[1]=ui->comboBox_6->currentIndex();
    emit sendTime(time);
}
