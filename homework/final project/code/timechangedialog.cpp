#include "timechangedialog.h"
#include "ui_timechangedialog.h"

timeChangeDialog::timeChangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::timeChangeDialog)
{
    ui->setupUi(this);
    ui->spinBox_1->setRange(2016,2016);
    ui->spinBox_2->setRange(11,11);
    ui->spinBox_3->setRange(1,15);
    ui->spinBox_4->setRange(0,24);
    ui->spinBox_5->setRange(0,30);
    ui->spinBox_5->setSingleStep(30);
    ui->spinBox_6->setRange(2016,2016);
    ui->spinBox_7->setRange(11,11);
    ui->spinBox_8->setRange(1,15);
    ui->spinBox_9->setRange(0,24);
    ui->spinBox_10->setRange(0,30);
    ui->spinBox_10->setSingleStep(30);
}

timeChangeDialog::~timeChangeDialog()
{
    delete ui;
}

void timeChangeDialog::receiveTime(int *t){
    for(int i=0; i<10; ++i){
        time[i]=t[i];
    }
    ui->spinBox_1->setValue(time[0]);
    ui->spinBox_2->setValue(time[1]);
    ui->spinBox_3->setValue(time[2]);
    ui->spinBox_4->setValue(time[3]);
    ui->spinBox_5->setValue(time[4]);
    ui->spinBox_6->setValue(time[5]);
    ui->spinBox_7->setValue(time[6]);
    ui->spinBox_8->setValue(time[7]);
    ui->spinBox_9->setValue(time[8]);
    ui->spinBox_10->setValue(time[9]);
    if(ui->spinBox_5->value()==0) ui->spinBox_5->setPrefix("0");
    if(ui->spinBox_10->value()==0) ui->spinBox_10->setPrefix("0");
}


void timeChangeDialog::on_spinBox_5_valueChanged(int arg1)
{
    if(arg1==0) ui->spinBox_5->setPrefix("0");
    else ui->spinBox_5->setPrefix("");
}

void timeChangeDialog::on_spinBox_10_valueChanged(int arg1)
{
    if(arg1==0) ui->spinBox_10->setPrefix("0");
    else ui->spinBox_10->setPrefix("");
}

void timeChangeDialog::on_buttonBox_accepted()
{
    time[0]=ui->spinBox_1->value();
    time[1]=ui->spinBox_2->value();
    time[2]=ui->spinBox_3->value();
    time[3]=ui->spinBox_4->value();
    time[4]=ui->spinBox_5->value();
    time[5]=ui->spinBox_6->value();
    time[6]=ui->spinBox_7->value();
    time[7]=ui->spinBox_8->value();
    time[8]=ui->spinBox_9->value();
    time[9]=ui->spinBox_10->value();

    //todo::ending time must later than starting time

    emit sendTime(time);
}
