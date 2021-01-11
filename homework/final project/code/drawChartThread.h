#ifndef DRAWCHARTTHREAD_H
#define DRAWCHARTTHREAD_H

#include <QThread>
#include <QString>
#include <QVector>
#include <qdebug.h>
#include <QStandardItemModel>
#include <QLineSeries>
#include <QDateTime>
#include <QtCharts>

struct PointData{
    int* x;
    double* y;
    int pointNum;
};

class thread_getDrawLineData: public QThread{
    Q_OBJECT
public:
    QStandardItemModel* data;
    int grid;
    int information_type;//0,1,2,3
    int graphic_type;//0,1,2,3
    int mode;//all area orgrid 0&1
    int tabNum;//0,1,2
    double timeStep;
    QDateTime *startDateTime;
    QDateTime *endDateTime;
    PointData* result;
    bool sentSuccess=false;
    double latitudeBorder[11]={30.524081949676066,30.55106155974085,30.57804116980564,30.605020779870426,
                         30.632000389935214,30.65898,30.685959610064785,30.712939220129574,30.73991883019436,
                         30.766898440259148,30.793878050323933};
    double lontitudeBorder[11]={103.90840747453143,103.93977117962514,103.97113488471885,104.00249858981257,
                               104.03386229490629,104.065226,104.0965897050937,104.12795341018742,
                               104.15931711528114,104.19068082037485,104.22204452546856};

signals:
    void sendPointData(PointData *);

public slots:
    void receiveParameters(QStandardItemModel* d, int g, int iT, int gT, int m, int t, double ts, QDateTime * st, QDateTime * et){
        data=d;
        grid=g;
        information_type=iT;
        graphic_type=gT;
        mode=m;
        tabNum=t;
        timeStep=ts;
        startDateTime=st;
        endDateTime=et;
    }

private:
    void run() override{
        qDebug()<<"drawline thread running";
        result=new PointData;
        result->x=new int[2500];
        result->y=new double[2500];
        for(int i=0; i<2500; ++i) result->y[i]=0;

        int step=timeStep*60*60;
        int xi;
        QDateTime dateTime_cur;
        QDateTime dateTime_thresh=startDateTime->addSecs(step);

        int row=0, gridRow=0, gridCol=0;
        if(mode==1){
            gridRow=grid/10;
            gridCol=grid%10;
        }
//        qDebug()<<"gridRow:"<<gridRow<<" gridCol:"<<gridCol;
        for(;row<data->rowCount(); ++row){
            //check if the data match the limitation
            dateTime_cur=QDateTime::fromTime_t(data->item(row, 1)->text().toInt());
            if(dateTime_cur.date()>endDateTime->date()) break;
            if(dateTime_cur>*endDateTime) continue;
            if(dateTime_cur<*startDateTime) continue;
            if(mode==1){
                if(data->item(row,3)->text().toDouble()<lontitudeBorder[gridCol] ||
                        data->item(row,3)->text().toDouble()>lontitudeBorder[gridCol+1]) continue;
                if(data->item(row,4)->text().toDouble()<latitudeBorder[gridRow] ||
                        data->item(row,4)->text().toDouble()>latitudeBorder[gridRow+1]) continue;
            }

            xi=startDateTime->secsTo(dateTime_cur);
            xi=int(xi/step);
            if(information_type==0) result->y[xi]++;
            else result->y[xi]+=data->item(row,7)->text().toDouble();
        }
        result->pointNum=startDateTime->secsTo(*endDateTime)/step;

        for (int i=0; i<result->pointNum; ++i){
            result->x[i]=((startDateTime->addSecs(i*step)).toTime_t());
            //qDebug()<<i<<' '<<result->x[i];
        }
        //todo: inteperate

        qDebug()<<"ready to send data";
        emit sendPointData(result);
        qDebug()<<"data sent";
    }
};

class thread_getDrawBarData: public QThread{
    Q_OBJECT
public:
    QStandardItemModel* data;
    int grid;
    int information_type;//0,1,2,3
    int graphic_type;//0,1,2,3
    int mode;//all area orgrid 0&1
    int tabNum;//0,1,2
    double timeStep;
    QDateTime *startDateTime;
    QDateTime *endDateTime;
    PointData* result;
    bool sentSuccess=false;
    double latitudeBorder[11]={30.524081949676066,30.55106155974085,30.57804116980564,30.605020779870426,
                         30.632000389935214,30.65898,30.685959610064785,30.712939220129574,30.73991883019436,
                         30.766898440259148,30.793878050323933};
    double longtitudeBorder[11]={103.90840747453143,103.93977117962514,103.97113488471885,104.00249858981257,
                               104.03386229490629,104.065226,104.0965897050937,104.12795341018742,
                               104.15931711528114,104.19068082037485,104.22204452546856};

signals:
    void sendPointData(PointData *);

public slots:
    void receiveParameters(QStandardItemModel* d, int g, int iT, int gT, int m, int t, double ts, QDateTime * st, QDateTime * et){
        data=d;
        grid=g;
        information_type=iT;
        graphic_type=gT;
        mode=m;
        tabNum=t;
        timeStep=ts;
        startDateTime=st;
        endDateTime=et;
    }

private:
    void run() override{
        qDebug()<<"drawBar thread running";
        result=new PointData;
        result->x=new int[100];
        result->y=new double[100];
        for(int i=0; i<100; ++i) result->y[i]=0;

        int row=0, gridRow=0, gridCol=0,xi;
        QDateTime dateTime_cur;
        double lng_cur, lat_cur;
        for(;row<data->rowCount(); ++row){
            //check if the data match the limitation
            dateTime_cur=QDateTime::fromTime_t(data->item(row, 1)->text().toInt());
            if(dateTime_cur.date()>endDateTime->date()) break;
            if(dateTime_cur>*endDateTime) continue;
            if(dateTime_cur<*startDateTime) continue;
            lng_cur=data->item(row,3)->text().toDouble();
            lat_cur=data->item(row,4)->text().toDouble();
            gridRow=0, gridCol=0;
            while(gridCol<=10 && lng_cur>longtitudeBorder[gridCol]) gridCol++;
            while(gridRow<=10 && lat_cur>latitudeBorder[gridRow]) gridRow++;
            if(gridCol==0 || gridCol==11 || gridRow==0 || gridRow==11) continue;
            gridCol--;
            gridRow--;
            xi=gridRow*10+gridCol;
            if(information_type==0) result->y[xi]++;
            else result->y[xi]+=data->item(row,7)->text().toDouble();
        }
        result->pointNum=100;

        for (int i=0; i<result->pointNum; ++i){
            result->x[i]=i;
//            qDebug()<<i<<' '<<result->y[i];
        }
        emit sendPointData(result);
    }
};

class thread_getDrawBarData2: public QThread{
    Q_OBJECT
public:
    QStandardItemModel* data;
    int grid;
    int information_type;//0,1,2,3
    int graphic_type;//0,1,2,3
    int mode;//all area orgrid 0&1
    int tabNum;//0,1,2
    double timeStep;
    QDateTime *startDateTime;
    QDateTime *endDateTime;
    PointData* result;
    bool sentSuccess=false;
    double latitudeBorder[11]={30.524081949676066,30.55106155974085,30.57804116980564,30.605020779870426,
                         30.632000389935214,30.65898,30.685959610064785,30.712939220129574,30.73991883019436,
                         30.766898440259148,30.793878050323933};
    double longtitudeBorder[11]={103.90840747453143,103.93977117962514,103.97113488471885,104.00249858981257,
                               104.03386229490629,104.065226,104.0965897050937,104.12795341018742,
                               104.15931711528114,104.19068082037485,104.22204452546856};

signals:
    void sendPointData(PointData *);

public slots:
    void receiveParameters(QStandardItemModel* d, int g, int iT, int gT, int m, int t, double ts, QDateTime * st, QDateTime * et){
        data=d;
        grid=g;
        information_type=iT;
        graphic_type=gT;
        mode=m;
        tabNum=t;
        timeStep=ts;
        startDateTime=st;
        endDateTime=et;
    }

private:
    void run() override{
        qDebug()<<"drawBar2 thread running";

        //todo:allow user to choose the point number

        result=new PointData;
        result->x=new int[21];
        result->y=new double[20];
        for(int i=0; i<10; ++i) result->y[i]=0;
        qDebug()<<data->rowCount();

        double minY[5], maxY[5], Y_cur;
        int row=0;
        QDateTime startTime_cur, endTime_cur;
        if(information_type==1){
            startTime_cur=QDateTime::fromTime_t(data->item(0, 1)->text().toInt());
            endTime_cur=QDateTime::fromTime_t(data->item(0, 2)->text().toInt());
            minY[0]=startTime_cur.secsTo(endTime_cur);
        }
        else{
            minY[0]=data->item(0,7)->text().toDouble();
        }
        maxY[0]=minY[0];
        for(row=1; row<data->rowCount(); ++row){
            if(information_type==1){
                Y_cur=data->item(row, 2)->text().toInt()-data->item(row, 1)->text().toInt();
            }
            else Y_cur=data->item(row,7)->text().toDouble();
            if(Y_cur<minY[0]) {
                for(int i=4; i>0; --i) minY[i]=minY[i-1];
                minY[0]=Y_cur;
            }
            if(Y_cur>maxY[0]) {
                for(int i=4; i>0; --i) maxY[i]=maxY[i-1];
                maxY[0]=Y_cur;
            }
        }
        qDebug()<<minY[0]<<' '<<maxY[0];

        int gridRow=0, gridCol=0;
        if(mode==1){
            gridRow=grid/10;
            gridCol=grid%10;
        }
        double xStep=(maxY[4]-minY[0])/10;
        for(int i=0; i<=10; ++i) {
            result->x[i]=int(minY[0]+xStep*i);
//            qDebug()<<"x["<<i<<"]:"<<result->x[i];
        }

        int xi;
        for(row=0;row<data->rowCount(); ++row){
            //check if the data match the limitation
            startTime_cur=QDateTime::fromTime_t(data->item(row, 1)->text().toInt());
            if(startTime_cur.date()>endDateTime->date()) break;
            if(startTime_cur>*endDateTime) continue;
            if(startTime_cur<*startDateTime) continue;
            if(mode==1){
                if(data->item(row,3)->text().toDouble()<longtitudeBorder[gridCol] ||
                        data->item(row,3)->text().toDouble()>longtitudeBorder[gridCol+1]) continue;
                if(data->item(row,4)->text().toDouble()<latitudeBorder[gridRow] ||
                        data->item(row,4)->text().toDouble()>latitudeBorder[gridRow+1]) continue;
            }
            if(information_type==1){
//                endTime_cur=QDateTime::fromTime_t(data->item(row, 2)->text().toInt());
//                Y_cur=startTime_cur.secsTo(endTime_cur);
                Y_cur=data->item(row, 2)->text().toInt()-data->item(row, 1)->text().toInt();
            }
            else Y_cur=data->item(row,7)->text().toDouble();
            xi=int((Y_cur-minY[0])/xStep);
            result->y[xi]++;
        }

//        for(int i=0; i<10; ++i) {
//            qDebug()<<"y["<<i<<"]:"<<result->y[i];
//        }
        result->pointNum=10;

        emit sendPointData(result);
    }
};

#endif // DRAWCHARTTHREAD_H
