#include "retrieveorderwindow.h"
#include "ui_retrieveorderwindow.h"
#include <qdebug.h>
#include <QMetaType>

retrieveOrderWindow::retrieveOrderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::retrieveOrderWindow)
{
    ui->setupUi(this);
    dataPresent=new QStandardItemModel();
    ui->tableView->setModel(dataPresent);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(select()));

    buttonGroup=new QButtonGroup;
    buttonGroup->addButton(ui->radioButton, 0);
    buttonGroup->addButton(ui->radioButton_2, 1);
    buttonGroup->addButton(ui->radioButton_3, 2);
    buttonGroup->addButton(ui->radioButton_4, 3);
    buttonGroup->setExclusive(false);
    connect(ui->radioButton, SIGNAL(toggled(bool)), this, SLOT(setEnabled_1(bool)));
    connect(ui->radioButton_2, SIGNAL(toggled(bool)), this, SLOT(setEnabled_2(bool)));
    connect(ui->radioButton_3, SIGNAL(toggled(bool)), this, SLOT(setEnabled_3(bool)));
    connect(ui->radioButton_4, SIGNAL(toggled(bool)), this, SLOT(setEnabled_4(bool)));
    ui->radioButton->setChecked(true);
    ui->radioButton_2->setChecked(true);
    ui->radioButton_2->setChecked(false);
    ui->radioButton_3->setChecked(true);
    ui->radioButton_4->setChecked(true);
    ui->radioButton_4->setChecked(false);

    //set the backButton
    ui->pushButton_3->setStyleSheet("border:none");

    setFixedSize(1000, 679);
}

retrieveOrderWindow::~retrieveOrderWindow()
{
    delete ui;
}

void retrieveOrderWindow::setEnabled_1(bool b){
    ui->label_13->setEnabled(b);
    ui->label_14->setEnabled(b);
    ui->lineEdit_001->setEnabled(b);
    ui->lineEdit_002->setEnabled(b);
}

void retrieveOrderWindow::setEnabled_2(bool b){
    ui->label_15->setEnabled(b);
    ui->label_16->setEnabled(b);
    ui->lineEdit_003->setEnabled(b);
    ui->lineEdit_004->setEnabled(b);
}

void retrieveOrderWindow::setEnabled_3(bool b){
    ui->label_17->setEnabled(b);
    ui->label_18->setEnabled(b);
    ui->label_19->setEnabled(b);
    ui->label_20->setEnabled(b);
    ui->lineEdit_005->setEnabled(b);
    ui->lineEdit_006->setEnabled(b);
    ui->lineEdit_007->setEnabled(b);
    ui->lineEdit_008->setEnabled(b);
}

void retrieveOrderWindow::setEnabled_4(bool b){
    ui->label_21->setEnabled(b);
    ui->label_22->setEnabled(b);
    ui->label_23->setEnabled(b);
    ui->label_24->setEnabled(b);
    ui->lineEdit_009->setEnabled(b);
    ui->lineEdit_010->setEnabled(b);
    ui->lineEdit_011->setEnabled(b);
    ui->lineEdit_012->setEnabled(b);
}

void retrieveOrderWindow::receiveData(QStandardItemModel* d){
    data=d;

    QStandardItem *tmp;
    for(int col=0; col<data->columnCount(); ++col){
        tmp=new QStandardItem(data->horizontalHeaderItem(col)->text());
        dataPresent->setHorizontalHeaderItem(col, tmp);
    }
}

void retrieveOrderWindow::select()
{
    //get the paremeter from ui page
    int startDateTimeLow, startDateTimeHigh, endDateTimeLow, endDateTimeHigh;
    double origLngLow, origLngHigh, origLatLow, origLatHigh, destLngLow, destLngHigh, destLatLow, destLatHigh;
    startDateTimeLow=QDateTime::fromString(ui->lineEdit_001->text(), "yyyy-M-d h:mm").toTime_t();
    startDateTimeHigh=QDateTime::fromString(ui->lineEdit_002->text(), "yyyy-M-d h:mm").toTime_t();
    endDateTimeLow=QDateTime::fromString(ui->lineEdit_003->text(), "yyyy-M-d h:mm").toTime_t();
    endDateTimeHigh=QDateTime::fromString(ui->lineEdit_004->text(), "yyyy-M-d h:mm").toTime_t();
//    qDebug()<<ui->lineEdit_001->text()<<' '<<startDateTimeLow<<' '<<endDateTimeHigh;
    origLngLow=ui->lineEdit_005->text().toDouble();
    origLngHigh=ui->lineEdit_006->text().toDouble();
    origLatLow=ui->lineEdit_007->text().toDouble();
    origLatHigh=ui->lineEdit_008->text().toDouble();
    destLngLow=ui->lineEdit_009->text().toDouble();
    destLngHigh=ui->lineEdit_010->text().toDouble();
    destLatLow=ui->lineEdit_011->text().toDouble();
    destLatHigh=ui->lineEdit_012->text().toDouble();

    dataPresent->clear();
    QStandardItem *tmp;
    for(int col=0; col<data->columnCount(); ++col){
        tmp=new QStandardItem(data->horizontalHeaderItem(col)->text());
        dataPresent->setHorizontalHeaderItem(col, tmp);
    }

    int startDateTime, endDateTime;
    QStandardItem *item;
    double origLng, origLat, destLng, destLat;
    int rowt=0,colNum=data->columnCount();
    for(int row=0; row<data->rowCount(); ++row){
        startDateTime=data->item(row,1)->text().toInt();
        endDateTime=data->item(row,2)->text().toInt();
        origLng=data->item(row,3)->text().toDouble();
        origLat=data->item(row,4)->text().toDouble();
        destLng=data->item(row,5)->text().toDouble();
        destLat=data->item(row,6)->text().toDouble();

//        if(row%1000==0)
//            qDebug()<<startDateTimeLow<<' '<<startDateTimeHigh<<' '<<endDateTimeLow<<' '<<endDateTimeHigh<<' '
//                   <<origLngLow<<' '<<origLngHigh<<' '<<origLatLow<<' '<<origLatHigh<<' '<<destLngLow<<' '<<destLngHigh
//                  <<' '<<destLatLow<<' '<<destLatHigh;
//        if(row%100==0)
//            qDebug()<< startDateTime<<' '<<endDateTime<<' '<<origLng<<' '<<origLat<<' '<<destLng<<' '<<destLat;
        if(ui->radioButton->isChecked()){
            if(startDateTime<startDateTimeLow || startDateTime>startDateTimeHigh) continue;
        }
        if(ui->radioButton_2->isChecked()){
            if(endDateTime<endDateTimeLow || endDateTime>endDateTimeHigh) continue;
        }
        if(ui->radioButton_3->isChecked()){
            if(origLng<origLngLow || origLng>origLngHigh) continue;
            if(origLat<origLatLow || origLat>origLatHigh) continue;
        }
        if(ui->radioButton_4->isChecked()){
            if(destLng<destLngLow || destLng>destLngHigh) continue;
            if(destLat<destLatLow || destLat>destLatHigh) continue;
        }

//        qDebug()<<rowt<<colNum;

        QStandardItem *item;
        item=new QStandardItem(data->item(row,0)->text());
        dataPresent->setItem(rowt, 0, item);
        item=new QStandardItem(QDateTime::fromTime_t(startDateTime).toString("M-d h:mm"));
        dataPresent->setItem(rowt, 1, item);
        item=new QStandardItem(QDateTime::fromTime_t(endDateTime).toString("M-d h:mm"));
        dataPresent->setItem(rowt, 2, item);
        for(int j=3; j<colNum; ++j){
            item=new QStandardItem(data->item(row,j)->text());
            dataPresent->setItem(rowt, j, item);
        }
        rowt++;
    }

    ui->tableView->repaint();
    QString str="total number: "+QString::number(rowt);
    ui->label_25->setText(str);

    //update();
}

void retrieveOrderWindow::on_pushButton_3_clicked()
{
    emit goBack();
    close();
}
