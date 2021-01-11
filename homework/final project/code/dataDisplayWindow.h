#ifndef DATADISPLAYWINDOW_H
#define DATADISPLAYWINDOW_H

#include <QMainWindow>
#include "timechangedialog.h"
#include "gridschangedialog.h"
#include <QStandardItemModel>
#include <QtCharts>
#include <QLineSeries>
#include "drawChartThread.h"
#include <QDateTimeAxis>

namespace Ui {
class dataDisplayWindow;
}

class dataDisplayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit dataDisplayWindow(QWidget *parent = nullptr);
    ~dataDisplayWindow();

signals:
    void sendTime(int *);
    void sendGrid(int);
    void sendData(QStandardItemModel* );
    void sendParameters(QStandardItemModel* d, int g, int iT, int gT, int m, int t, double ts, QDateTime * st, QDateTime * et);
        //sendParameters(data, grid,information_type, graphic_type, mode, tabNum, timeStep, startDateTime, endDateTime);
    void setInit(int);
    void draw(bool);
    void signalReceived(bool);
    void goBack();

//public slots:

private slots:
    void on_pushButton_clicked();
    void receiveTime(int *);
    void on_radioButton_2_toggled();
    void on_pushButton_2_clicked();
    void receiveGrid(int);
    void receiveData(QStandardItemModel*);
    void initLayout(int);
    void drawchart(bool);
    void drawLine(PointData*);
    void drawBar(PointData*);
    void drawBar2(PointData*);
    void on_radioButton_toggled(bool checked);
    void on_radioButton_3_toggled(bool checked);
    void on_radioButton_4_toggled(bool checked);
    void on_radioButton_5_toggled(bool checked);
    void on_radioButton_6_toggled(bool checked);
    void on_radioButton_7_toggled(bool checked);
    void on_radioButton_8_toggled(bool checked);
    void on_radioButton_9_toggled(bool checked);
    void on_radioButton_10_toggled(bool checked);
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_3_clicked();

private:
    Ui::dataDisplayWindow *ui;
    int time[10]={2016,11,2,8,0,2016,11,3,9,0};
    timeChangeDialog *timeDialog;
    gridsChangeDialog *gridsDialog;
    int grid;
    QButtonGroup* informationButton;
    QButtonGroup* graphicButton;
    QButtonGroup* gridsButton;
    int information_type;//0,1,2,3
    int graphic_type;//0,1,2,3
    int mode;//all area orgrid 0&1
    double timeStep;
    QStandardItemModel* data;
    int tabNum;
    QDateTime *startDateTime;
    QDateTime *endDateTime;

    QChart *chart1;
    QChart *chart2;
    QChart *chart3;
    QSplineSeries *series_line;
    QBarSeries *series_bar;
    QBarSeries *series_bar2;
    QBarSet *set_bar;
    QBarSet *set_bar2;
    QPieSeries *series_pie;
    QPieSeries *series_pie2;
    QScatterSeries *series_scatter;
    QDateTimeAxis *axisX_time;
    QBarCategoryAxis *axisX_grids;
    QBarCategoryAxis *axisX_tresh;
    QStringList axisX_grids_categories={};
    QStringList axisX_tresh_categories={};
    QValueAxis *axisY_number;
    QValueAxis *axisY_number2;
    QValueAxis *axisY_number3;

    thread_getDrawLineData* thread_Line;
    thread_getDrawBarData* thread_Bar;
    thread_getDrawBarData2* thread_Bar2;
    PointData* pointData;

    bool initDone;
};

#endif // DATADISPLAYWINDOW_H
