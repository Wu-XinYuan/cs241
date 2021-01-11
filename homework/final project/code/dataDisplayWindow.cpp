#include "dataDisplayWindow.h"
#include "ui_dataDisplayWindow.h"
#include <QString>
#include <QButtonGroup>
#include <QDate>
#include <QDateTime>
#include <qdebug.h>
#include "drawChartThread.h"

dataDisplayWindow::dataDisplayWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dataDisplayWindow)
{
    ui->setupUi(this);
    timeDialog=new timeChangeDialog;
    connect(this, SIGNAL(sendTime(int *)), timeDialog, SLOT(receiveTime(int *)));
    connect(timeDialog, SIGNAL(sendTime(int *)), this, SLOT(receiveTime(int *)));

    grid=0;
    gridsDialog=new gridsChangeDialog;
    connect(this, SIGNAL(sendGrid(int)), gridsDialog, SLOT(receiveGrid(int)));
    connect(gridsDialog, SIGNAL(sendGrid(int)), this, SLOT(receiveGrid(int)));

    //set up button groups
    informationButton=new QButtonGroup;
    graphicButton=new QButtonGroup;
    gridsButton=new QButtonGroup;
    informationButton->addButton(ui->radioButton_3, 0);
    informationButton->addButton(ui->radioButton_4, 1);
    informationButton->addButton(ui->radioButton_5, 2);
    informationButton->addButton(ui->radioButton_6, 3);
    graphicButton->addButton(ui->radioButton_7, 0);
    graphicButton->addButton(ui->radioButton_8, 1);
    graphicButton->addButton(ui->radioButton_9, 2);
    graphicButton->addButton(ui->radioButton_10, 3);
    gridsButton->addButton(ui->radioButton, 0);
    gridsButton->addButton(ui->radioButton_2, 1);

    //set the backButton
    ui->pushButton_3->setStyleSheet("border:none");

    //set up combox
    ui->comboBox->addItem("0.25",25);
    ui->comboBox->addItem("0.5",50);
    ui->comboBox->addItem("1", 100);
    ui->comboBox->addItem("2", 200);
    ui->comboBox->addItem("5",500);

    //set up graphics
    chart1=new QChart;
    chart2=new QChart;
    chart3=new QChart;
    series_line=new QSplineSeries;
    series_bar=new QBarSeries;
    series_bar2=new QBarSeries;
    series_pie= new QPieSeries;
    series_pie2= new QPieSeries;
    series_scatter= new QScatterSeries;
    ui->charView_incline->setChart(chart1);
    ui->charView_distg->setChart(chart2);
    ui->charView_dist->setChart(chart3);

    //set up date
    startDateTime=new QDateTime;
    endDateTime=new QDateTime;

    //connect with chart-drawing thread
    thread_Line=new thread_getDrawLineData;
    thread_Bar=new thread_getDrawBarData;
    set_bar=new QBarSet("value");
    set_bar2=new QBarSet("number");
    thread_Bar2=new thread_getDrawBarData2;
    pointData=new PointData;
    for(int i=0; i<100; ++i) axisX_grids_categories.append(QString::number(i));
    axisX_time=new QDateTimeAxis;
    axisX_grids=new QBarCategoryAxis;
    axisX_tresh= new QBarCategoryAxis;
    axisX_grids->append(axisX_grids_categories);
    axisX_tresh->append(axisX_tresh_categories);
    axisY_number=new QValueAxis;
    axisY_number2=new QValueAxis;
    axisY_number3=new QValueAxis;
    connect(this, SIGNAL(sendParameters(QStandardItemModel*, int, int, int, int, int, double, QDateTime *, QDateTime *)), thread_Line, SLOT(receiveParameters(QStandardItemModel*, int, int, int, int, int, double, QDateTime *, QDateTime *)));
    connect(this, SIGNAL(sendParameters(QStandardItemModel*, int, int, int, int, int, double, QDateTime *, QDateTime *)), thread_Bar, SLOT(receiveParameters(QStandardItemModel*, int, int, int, int, int, double, QDateTime *, QDateTime *)));
    connect(this, SIGNAL(sendParameters(QStandardItemModel*, int, int, int, int, int, double, QDateTime *, QDateTime *)), thread_Bar2, SLOT(receiveParameters(QStandardItemModel*, int, int, int, int, int, double, QDateTime *, QDateTime *)));
    connect(thread_Line, SIGNAL(sendPointData(PointData *)), this, SLOT(drawLine(PointData*)));
    connect(thread_Bar, SIGNAL(sendPointData(PointData *)), this, SLOT(drawBar(PointData*)));
    connect(thread_Bar2, SIGNAL(sendPointData(PointData *)), this, SLOT(drawBar2(PointData*)));

    //other initialization
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(initLayout(int)));
    connect(this, SIGNAL(setInit(int)), this, SLOT(initLayout(int)));
    ui->tabWidget->setCurrentIndex(0);
    initDone=false;
    emit setInit(0);
    connect(this, SIGNAL(draw(bool)), this, SLOT(drawchart(bool)));

    setFixedSize(1000, 679);

    update();
//    emit draw(true);
}

dataDisplayWindow::~dataDisplayWindow()
{
    delete ui;
//    emit destroyed();
}

void dataDisplayWindow::on_pushButton_clicked()
{
    emit sendTime(time);
    timeDialog->exec();
}

void dataDisplayWindow::receiveTime(int *t){
    for(int i=0; i<10; ++i) time[i]=t[i];

    //todo: if time didn't change return

    //show the time in lael
    QString startt="", endt="";
    for(int i=0; i<2; ++i){
        startt=startt+QString::number(time[i])+"-";
        endt=endt+QString::number(time[i+5])+"-";
    }
    startt=startt+QString::number(time[2])+" ";
    endt=endt+QString::number(time[7])+" ";
    startt=startt+QString::number(time[3])+":";
    endt=endt+QString::number(time[8])+":";
    startt+=QString::number(time[4]);
    endt+=QString::number(time[9]);
    if(time[4]==0) startt+="0";
    if(time[9]==0) endt+="0";
    ui->label_2->setText(startt);
    ui->label_4->setText(endt);

    emit draw(true);
}

void dataDisplayWindow::receiveGrid(int g){
    int fg=grid;
    grid=g;
    ui->radioButton_2->setText("grid:"+QString::number(grid));
    emit draw(g!=fg);
}

void dataDisplayWindow::receiveData(QStandardItemModel* d){
    data=d;
    emit draw(true);
}

void dataDisplayWindow::drawchart(bool b){
    if (!b) return;
    if (!initDone) return;

    //get parameters
    information_type=informationButton->checkedId();
    graphic_type=graphicButton->checkedId();
    timeStep=(ui->comboBox->currentData().toInt())/100.0;
    tabNum=ui->tabWidget->currentIndex();
    QDate startDate(time[0], time[1], time[2]);
    QTime startTime(time[3], time[4]);
    QDate endDate(time[5], time[6], time[7]);
    QTime endTime(time[8], time[9]);
    startDateTime->setDate(startDate);
    startDateTime->setTime(startTime);
    endDateTime->setDate(endDate);
    endDateTime->setTime(endTime);
    mode=gridsButton->checkedId();

//    qDebug()<<data<<' '<<grid<<' '<<information_type<<' '<<graphic_type<<' '<<mode<<' '<<tabNum<<' '<<timeStep<<' '<<startDateTime->toString()<<' '<<endDateTime->toString();

    //send signals
    emit sendParameters(data, grid,information_type, graphic_type, mode, tabNum, timeStep, startDateTime, endDateTime);
    switch (graphic_type) {
    case 0:case 1:{
        thread_Line->start();
        break;
    }
    case 2:case 3:{
        if(tabNum==1){
           thread_Bar->start();
        }
        else{
         thread_Bar2->start();
        }
        break;
    }
    }
}

void dataDisplayWindow::initLayout(int op){
    initDone=false;
    switch(op){
    case 0:{
        ui->comboBox->setEnabled(true);
        ui->comboBox->setCurrentText("1");
        ui->label_5->setEnabled(true);
        ui->label_6->setEnabled(true);
        ui->pushButton_2->setEnabled(false);
        ui->radioButton_2->setEnabled(true);
        ui->radioButton_3->setEnabled(true);
        ui->radioButton_4->setEnabled(false);
        ui->radioButton_5->setEnabled(false);
        ui->radioButton_6->setEnabled(true);
        ui->radioButton_7->setEnabled(true);
        ui->radioButton_8->setEnabled(true);
        ui->radioButton_9->setEnabled(false);
        ui->radioButton_10->setEnabled(false);
        ui->radioButton->setChecked(true);
        ui->radioButton_3->setChecked(true);
        ui->radioButton_7->setChecked(true);
        break;
    }
    case 1:{
        ui->comboBox->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->label_6->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->radioButton_2->setEnabled(false);
        ui->radioButton_3->setEnabled(true);
        ui->radioButton_4->setEnabled(false);
        ui->radioButton_5->setEnabled(false);
        ui->radioButton_6->setEnabled(true);
        ui->radioButton_7->setEnabled(false);
        ui->radioButton_8->setEnabled(false);
        ui->radioButton_9->setEnabled(true);
        ui->radioButton_10->setEnabled(true);
        ui->radioButton->setChecked(true);
        ui->radioButton_3->setChecked(true);
        ui->radioButton_9->setChecked(true);
        break;
    }
    case 2:{
        ui->comboBox->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->label_6->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->radioButton_2->setEnabled(true);
        ui->radioButton_3->setEnabled(false);
        ui->radioButton_4->setEnabled(true);
        ui->radioButton_5->setEnabled(false);
        ui->radioButton_6->setEnabled(false);
        ui->radioButton_7->setEnabled(false);
        ui->radioButton_8->setEnabled(false);
        ui->radioButton_9->setEnabled(true);
        ui->radioButton_10->setEnabled(true);
        ui->radioButton->setChecked(true);
        ui->radioButton_4->setChecked(true);
        ui->radioButton_9->setChecked(true);
        break;
    }
    }
    initDone=true;
    emit draw(true);
}



void dataDisplayWindow::drawLine(PointData* p){
//    thread_Line->exit();

    pointData->x=p->x;
    pointData->y=p->y;
    pointData->pointNum=p->pointNum;

    qDebug()<<"in draw line";
    qDebug()<<"information type"<<information_type<<'\n';
    chart1->removeSeries(series_line);
    chart1->removeSeries(series_scatter);
    chart1->removeAxis(axisX_time);
    chart1->removeAxis(axisY_number);
    series_line->clear();
    series_scatter->clear();

    //set up the chart
    int cnt_x=pointData->pointNum;
    QDateTime curX;
    for(int i=0; i<cnt_x; i++) {
        curX=QDateTime::fromTime_t(pointData->x[i]);
        if(graphic_type==0){
            series_line->append(curX.toMSecsSinceEpoch(),pointData->y[i]);
        }
        else{
            series_scatter->append(curX.toMSecsSinceEpoch(),pointData->y[i]);
        }
    }
    axisX_time->setMin(*startDateTime);
    axisX_time->setMax(*endDateTime);
    axisX_time->setTitleText("time");
    axisX_time->setFormat("dd h:mm");
    axisX_time->setTickCount(4+graphic_type);
    double maxY=0, minY=pointData->y[0];
    for(int i=0; i<pointData->pointNum; ++i){
        if(pointData->y[i]>maxY) maxY=pointData->y[i];
        if(pointData->y[i]<minY) minY=pointData->y[i];
    }
    axisY_number->setTickCount(4+graphic_type*2);
    if(information_type==0){
        if(maxY-minY<6){
            axisY_number->setRange(minY,maxY);
            axisY_number->setTickCount(maxY-minY+1);
        }
        else axisY_number->setRange(int(minY*0.75),int(maxY*1.1));
        axisY_number->setLabelFormat("%d");
        axisY_number->setTitleText("orders number");
    }
    else{
        axisY_number->setRange(minY*0.75,maxY*1.1);
        axisY_number->setLabelFormat("%.1f");
        axisY_number->setTitleText("fee");
    }
    if(graphic_type==0){
        chart1->addSeries(series_line);
        chart1->setAxisX(axisX_time, series_line);
        chart1->setAxisY(axisY_number, series_line);
    }
    else{
        series_scatter->setMarkerSize(20-5*(pointData->pointNum>10)-5*(pointData->pointNum>20)-2*(pointData->pointNum>50));
        chart1->addSeries(series_scatter);
        chart1->setAxisX(axisX_time, series_scatter);
        chart1->setAxisY(axisY_number, series_scatter);
    }
    QString title="relationship between ";
    if(information_type==0) title+="orders number";
    else title+="total fee";
    title+=" and time";
    chart1->setTitle(title);
    chart1->legend()->setVisible(false);
    ui->charView_incline->repaint();
}

void dataDisplayWindow::drawBar(PointData* p){
    pointData->x=p->x;
    pointData->y=p->y;
    pointData->pointNum=p->pointNum;

    qDebug()<<"in draw Bar";
    chart2->removeSeries(series_bar);
    chart2->removeSeries(series_pie);
    chart2->removeAxis(axisX_grids);
    chart2->removeAxis(axisY_number2);
    series_bar->clear();
    set_bar=new QBarSet("value");
    series_pie->clear();

    //set up the chart
    int cnt_x=pointData->pointNum;
    for(int i=0; i<cnt_x; i++) {
        if(graphic_type==2){
            *set_bar<<(pointData->y[i]);
            axisX_grids->setRange("0","99");
        }
        else{
            series_pie->append(QString::number(i),pointData->y[i]);
        }
    }
    axisX_grids->setTitleText("grid id");
//    axisX_grids->setTickCount(4+graphic_type);
    double maxY=0, minY=pointData->y[0];
    for(int i=0; i<pointData->pointNum; ++i){
        if(pointData->y[i]>maxY) maxY=pointData->y[i];
        if(pointData->y[i]<minY) minY=pointData->y[i];
    }
    axisY_number2->setTickCount(4+graphic_type*2);
    if(information_type==0){
        if(maxY-minY<6){
            axisY_number2->setRange(minY,maxY);
            axisY_number2->setTickCount(maxY-minY+1);
        }
        else axisY_number2->setRange(minY,int(maxY*1.05));
        axisY_number2->setLabelFormat("%d");
        axisY_number2->setTitleText("orders number");
    }
    else{
        axisY_number2->setRange(minY,maxY*1.1);
        axisY_number2->setLabelFormat("%.1f");
        axisY_number2->setTitleText("total revenue");
    }
    if(graphic_type==2){
        series_bar->append(set_bar);
        chart2->addSeries(series_bar);
        chart2->setAxisX(axisX_grids, series_bar);
        chart2->setAxisY(axisY_number2, series_bar);
        ui->charView_distg->resize(3000,480);
        ui->scrollAreaWidgetContents->setMinimumWidth(3020);
    }
    else{
        chart2->addSeries(series_pie);
        ui->charView_distg->resize(671,480);
        ui->scrollAreaWidgetContents->setMinimumWidth(500);
    }
    QString title="relationship between ";
    if(information_type==0) title+="orders number";
    else title+="total fee";
    title+=" and grids";
    chart2->setTitle(title);
    ui->charView_distg->repaint();
}

void dataDisplayWindow::drawBar2(PointData* p){
    pointData->x=p->x;
    pointData->y=p->y;
    pointData->pointNum=p->pointNum;

    qDebug()<<"in draw Bar2";
    chart3->removeSeries(series_bar2);
    chart3->removeSeries(series_pie2);
//    chart3->removeAxis(axisX_tresh);
    chart3->removeAxis(axisY_number3);
    series_bar2->clear();
    set_bar2=new QBarSet("number");
    series_pie2->clear();

    //set up the chart
    int cnt_x=pointData->pointNum;
    for(int i=0; i<cnt_x; i++) {
        if(graphic_type==2){
            *set_bar2<<(pointData->y[i]);
        }
        else{
            series_pie2->append(QString::number(pointData->x[i]),pointData->y[i]);
        }
    }
    if(graphic_type==2){
        axisX_tresh->clear();
        axisX_tresh_categories.clear();
        for(int i=0; i<pointData->pointNum-1; ++i)
            axisX_tresh_categories<<('~'+QString::number(pointData->x[i+1]/60));
        axisX_tresh->append(axisX_tresh_categories);
        axisX_tresh->setRange('~'+QString::number(pointData->x[1]/60),'~'+QString::number(pointData->x[10]/60));
    }
    double maxY=0, minY=pointData->y[0];
    for(int i=0; i<pointData->pointNum; ++i){
        if(pointData->y[i]>maxY) maxY=pointData->y[i];
        if(pointData->y[i]<minY) minY=pointData->y[i];
    }
    axisY_number3->setTickCount(4+graphic_type*2);
    if(maxY-minY<6){
        axisY_number3->setRange(minY,maxY);
        axisY_number3->setTickCount(maxY-minY+1);
    }
    else axisY_number3->setRange(0,int(maxY*1.05));
    axisY_number3->setLabelFormat("%d");
    axisY_number3->setTitleText("orders number");
    if(graphic_type==2){
        series_bar2->append(set_bar2);
        chart3->addSeries(series_bar2);
        axisX_tresh->setTitleText("time range(min)");
        chart3->setAxisX(axisX_tresh, series_bar2);
        chart3->setAxisY(axisY_number3, series_bar2);
    }
    else{
        chart3->addSeries(series_pie2);
    }
    QString title="distribution of ";
    if(information_type==1) title+="travel time";
    else title+="fee";
    chart3->setTitle(title);
    ui->charView_dist->repaint();
    ui->charView_dist->setRenderHint(QPainter::Antialiasing);
}

void dataDisplayWindow::on_radioButton_toggled(bool checked)
{
    if(checked) emit draw(true);
}

void dataDisplayWindow::on_radioButton_2_toggled( )
{
    if (ui->radioButton_2->isChecked()){
        ui->pushButton_2->setEnabled(true);
        emit draw(true);
    }
    else ui->pushButton_2->setEnabled(false);
}

void dataDisplayWindow::on_pushButton_2_clicked()
{
    emit sendGrid(grid);
    gridsDialog->exec(); 
}

void dataDisplayWindow::on_radioButton_3_toggled(bool checked)
{
    if(checked) emit draw(true);
}

void dataDisplayWindow::on_radioButton_4_toggled(bool checked)
{
    if(checked) emit draw(true);
}

void dataDisplayWindow::on_radioButton_5_toggled(bool checked)
{
    if(checked) emit draw(true);
}

void dataDisplayWindow::on_radioButton_6_toggled(bool checked)
{
    if(checked) emit draw(true);
}

void dataDisplayWindow::on_radioButton_7_toggled(bool checked)
{
    if(checked) emit draw(true);
}

void dataDisplayWindow::on_radioButton_8_toggled(bool checked)
{
    if(checked) emit draw(true);
}

void dataDisplayWindow::on_radioButton_9_toggled(bool checked)
{
    if(checked) emit draw(true);
}

void dataDisplayWindow::on_radioButton_10_toggled(bool checked)
{
    if(checked) emit draw(true);
}

void dataDisplayWindow::on_comboBox_currentIndexChanged(int index)
{
    emit draw(true);
}

void dataDisplayWindow::on_pushButton_3_clicked()
{
    emit goBack();
    close();
}
