#include "startingWindow.h"
#include "ui_startingwindow.h"
#include <QString>
#include <QFile>
#include <QThread>
#include <qDebug>
#include"MyThread.h"
#include "timechoicedialog.h"
#include "dataDisplayWindow.h"

StartingWindow::StartingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartingWindow)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    ui->label->setVisible(false);
    d=new TimeChoicedDialog;
    dD=new dataDisplayWindow;
    aD=new AnalysisWindow;
    rO=new retrieveOrderWindow;
    connect(aD, SIGNAL(goBack()), this, SLOT(showThis()));
    connect(dD, SIGNAL(goBack()), this, SLOT(showThis()));
    connect(rO, SIGNAL(goBack()), this, SLOT(showThis()));

    setFixedSize(1000, 679);
}

StartingWindow::~StartingWindow()
{
    delete ui;
}

void StartingWindow::thread_change(int i){
    threadNum+=i;
}

void StartingWindow::openDataDisplay(bool b){
    dD->show();
//    dD->SetupRightAfterStart();
    this->hide();
}

void StartingWindow::openAnalyze(bool b){
    aD->show();
    this->hide();
}

void StartingWindow::openRetrieve(bool b){
    rO->show();
    this->hide();
}

void StartingWindow::on_pushButton_clicked()
{
    int i=d->exec();
    if (i==0) return;

    ui->progressBar->setVisible(true);
    ui->label->setVisible(true);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->label_4->setVisible(false);


    thread1 *thread1_1=new thread1;
    QVector<QString> filename;
    QVector<int> date;
    QString file_path="../Dataset", fname;
    //TODO::let user choose the path
    for(int day=1; day<=15; ++day){
        fname=file_path+"/order_201611";
        if(day<10) fname+="0";
        fname+=QString::number(day);
        fname+="_part";
        for(int part=0; part<=4; ++part){
            filename.push_back(fname+QString::number(part)+".csv");
            date.push_back(day);
        }
    }
    thread1_1->filename=filename;
    thread1_1->day=date;
    connect(thread1_1, SIGNAL(changed(int)),ui->progressBar,SLOT(setValue(int)));
    connect(thread1_1, SIGNAL(text(QString)),ui->label,SLOT(setText(QString)));
    connect(thread1_1, SIGNAL(done(bool)), this, SLOT(openDataDisplay(bool)));
    connect(thread1_1, SIGNAL(sendData(QStandardItemModel*)), dD, SLOT(receiveData(QStandardItemModel*)));

    thread1_1->start();
//    thread1_1->wait();

}

void StartingWindow::on_pushButton_2_clicked(){
    ui->progressBar->setVisible(true);
    ui->label->setVisible(true);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->label_4->setVisible(false);


    thread1 *thread1_1=new thread1;
    QVector<QString> filename;
    QVector<int> date;
    QString file_path="../Dataset", fname;
    //TODO::let user choose the path
    //15
    for(int day=1; day<=15; ++day){
        fname=file_path+"/order_201611";
        if(day<10) fname+="0";
        fname+=QString::number(day);
        fname+="_part";
        for(int part=0; part<=4; ++part){
            filename.push_back(fname+QString::number(part)+".csv");
            date.push_back(day);
        }
    }
    thread1_1->filename=filename;
    thread1_1->day=date;
    connect(thread1_1, SIGNAL(changed(int)),ui->progressBar,SLOT(setValue(int)));
    connect(thread1_1, SIGNAL(text(QString)),ui->label,SLOT(setText(QString)));
    connect(thread1_1, SIGNAL(done(bool)), this, SLOT(openAnalyze(bool)));
    connect(thread1_1, SIGNAL(sendData(QStandardItemModel*)), aD, SLOT(receiveData(QStandardItemModel*)));

    thread1_1->start();
}

void StartingWindow::on_pushButton_3_clicked(){
    ui->progressBar->setVisible(true);
    ui->label->setVisible(true);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->label_4->setVisible(false);


    thread1 *thread1_1=new thread1;
    QVector<QString> filename;
    QVector<int> date;
    QString file_path="../Dataset", fname;
    //TODO::let user choose the path
    for(int day=1; day<=15; ++day){
        fname=file_path+"/order_201611";
        if(day<10) fname+="0";
        fname+=QString::number(day);
        fname+="_part";
        for(int part=0; part<=4; ++part){
            filename.push_back(fname+QString::number(part)+".csv");
            date.push_back(day);
        }
    }
    thread1_1->filename=filename;
    thread1_1->day=date;
    connect(thread1_1, SIGNAL(changed(int)),ui->progressBar,SLOT(setValue(int)));
    connect(thread1_1, SIGNAL(text(QString)),ui->label,SLOT(setText(QString)));
    connect(thread1_1, SIGNAL(done(bool)), this, SLOT(openRetrieve(bool)));
    connect(thread1_1, SIGNAL(sendData(QStandardItemModel*)), rO, SLOT(receiveData(QStandardItemModel*)));

    thread1_1->start();
}

void StartingWindow::showThis( ){
    qDebug()<<"in show";
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    ui->label->setVisible(false);
    ui->pushButton->setVisible(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton_3->setVisible(true);
    ui->label_4->setVisible(true);
    this->setVisible(true);
}
