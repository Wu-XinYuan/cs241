#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QBrush>
#include <QPainter>

struct Point
{
    int posX;
    int posY;
    int count;
};

namespace Ui {
class AnalysisWindow;
}

class AnalysisWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AnalysisWindow(QWidget *parent = nullptr);
    ~AnalysisWindow();

private slots:
    void receiveData(QStandardItemModel*);
    void on_horizontalSlider_valueChanged(int position);
    void on_pushButton_3_clicked();

signals:
    void goBack();

private:
    Ui::AnalysisWindow *ui;
    QStandardItemModel* data;
    int currentDate;
    int currentHour;

    static constexpr int HeatAlpha=200;
    QRgb colorList[256];
    QImage dataImg;
    QImage heatImg;
    int imgWidth=619;
    int imgHeight=628;
    int *countTable=new int[imgWidth*imgHeight];
    int maxCount;
    int pointCount;

    void drawThermal();
    int *xx;
    int *yy;
    int *time;


};

#endif // ANALYSISWINDOW_H
