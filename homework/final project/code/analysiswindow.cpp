#include "analysiswindow.h"
#include "ui_analysiswindow.h"
#include <QBrush>
#include <QPainter>
#include <QDateTime>

AnalysisWindow::AnalysisWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalysisWindow)
{
    ui->setupUi(this);
//    ui->quickWidget->setSource(QUrl("../final_project/map.html"));
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(360);
    ui->horizontalSlider->setSingleStep(1);


    //set up the color for paint
    dataImg=QImage(619,628,QImage::Format_Alpha8);
    dataImg.fill(Qt::transparent);
    heatImg=QImage(619,628,QImage::Format_ARGB32);
    heatImg.fill(Qt::transparent);
    QLinearGradient linear=QLinearGradient(QPoint(0,0),QPoint(255,0));
    linear.setColorAt(0, Qt::blue);
    linear.setColorAt(0.4, Qt::blue);
    linear.setColorAt(0.5, Qt::cyan);
    linear.setColorAt(0.6, Qt::green);
    linear.setColorAt(0.8, Qt::yellow);
    linear.setColorAt(0.95, Qt::red);
    QImage img(256,1,QImage::Format_ARGB32);
    QPainter painter(&img);
    painter.fillRect(img.rect(),linear);
    int alpha=0;
    for(int i=0;i<256;i++){
        alpha=HeatAlpha/255.0*i;
        colorList[i]=img.pixel(i,0)&0x00FFFFFF|(alpha<<24);
    }

    //set the backButton
    ui->pushButton_3->setStyleSheet("border:none");

    setFixedSize(1000, 679);
}

AnalysisWindow::~AnalysisWindow()
{
    delete ui;
}

void AnalysisWindow::receiveData(QStandardItemModel* d){
    data=d;
    double lngMin=103.85, lngMax=104.25, latMin=30.5, latMax=30.85;

    //preprocess the data
    int cnt=data->rowCount();
    xx=new int[cnt];
    yy=new int[cnt];
    time=new int[cnt];
    double lngStep=(lngMax-lngMin)/619, latStep=(latMax-latMin)/628, timeStep=3600;
    int tx,ty;
    QDate startQDate(2016,11,1);
    QTime startQTime(0,0);
    QDateTime startDateTime(startQDate, startQTime);
    int startTime=startDateTime.toTime_t();
    for(int row=0; row<cnt; ++row){
        tx=int((data->item(row,3)->text().toDouble()-103.85)/lngStep);
        ty=imgHeight-int((data->item(row,4)->text().toDouble()-30.5)/latStep);
        if(tx>imgWidth) tx=-1;
        if(ty>imgHeight) ty=-1;
        xx[row]=tx;
        yy[row]=ty;

        time[row]=int((data->item(row, 1)->text().toInt()-startTime)/timeStep);
    }
    ui->horizontalSlider->setValue(8);
}

void AnalysisWindow::on_horizontalSlider_valueChanged(int position)
{
    currentDate=position/24+1;
    currentHour=position%24;
    QString text="2016-11-"+QString::number(currentDate)+" "+QString::number(currentHour)+":00";
    ui->label_3->setText(text);

    //set up the point data to draw;
    maxCount=1;
    memset(countTable,0,imgWidth*imgHeight*sizeof(int));
    for(int i=0; i<data->rowCount(); ++i){
        if(time[i]==position){
            if(xx[i]<0 || yy[i]<0) continue;
            countTable[xx[i]+yy[i]*imgWidth]++;
            if(countTable[xx[i]+yy[i]*imgWidth]>maxCount) maxCount=countTable[xx[i]+yy[i]*imgWidth];
        }
    }
    drawThermal();
}

void AnalysisWindow::drawThermal(){
    dataImg.fill(Qt::transparent);
    heatImg.fill(Qt::transparent);

    //draw dataImg
    QPainter painter(&dataImg);
    painter.setPen(Qt::transparent);
    int radius=15;
    for (int i=0; i<imgHeight; ++i){
        for(int j=0; j<imgWidth; ++j){
            int k=i*imgWidth+j;
            if(countTable[k]==0) continue;
            const uchar alpha=uchar(countTable[k]*255.0/maxCount);
            QRadialGradient gradient(j,i,radius);
            gradient.setColorAt(0,QColor(0,0,0,alpha));
            gradient.setColorAt(1,QColor(0,0,0,0));
            painter.setBrush(gradient);
            painter.drawEllipse(QPointF(j,i),radius,radius);
        }
    }

    //draw heatImg
    for(int row=0;row<dataImg.height();row++)
    {
        const uchar *line_data=dataImg.scanLine(row);
        QRgb *line_heat=reinterpret_cast<QRgb*>(heatImg.scanLine(row));
        for(int col=0;col<dataImg.width();col++)
        {
            line_heat[col]=colorList[line_data[col]];
        }
    }

    ui->label_4->setPixmap(QPixmap::fromImage(heatImg));
//    update();
}

void AnalysisWindow::on_pushButton_3_clicked()
{
    emit goBack();
    close();
}
