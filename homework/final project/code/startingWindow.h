#ifndef STARTINGWINDOW_H
#define STARTINGWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "timechoicedialog.h"
#include "dataDisplayWindow.h"
#include "analysiswindow.h"
#include <retrieveorderwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class StartingWindow; }
QT_END_NAMESPACE

class StartingWindow : public QMainWindow
{
    Q_OBJECT

public:
    StartingWindow(QWidget *parent = nullptr);
    ~StartingWindow();
    int threadNum=0;

private:
    QStandardItemModel *data=nullptr;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void thread_change(int);
    void openDataDisplay(bool);
    void openAnalyze(bool);
    void openRetrieve(bool);
    void showThis();

private:
    Ui::StartingWindow *ui;
    TimeChoicedDialog *d;
    dataDisplayWindow *dD;
    AnalysisWindow *aD;
    retrieveOrderWindow *rO;
};
#endif // STARTINGWINDOW_H
