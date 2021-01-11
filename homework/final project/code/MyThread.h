#ifndef MYTHREAD_H
#define MYTHREAD_H

#include<QThread>
#include<QFile>
#include<QString>
#include<QVector>
#include<qdebug.h>
#include <QStandardItemModel>

class thread1 : public QThread{
    Q_OBJECT
public:
    QVector<QString> filename;
    QVector<int> day;
signals:
    void changed(int);
    void text(QString);
    void change(int);
    void sendData(QStandardItemModel*);
    void done(bool);
private:
    void run() override{

        emit change(1);
        int file_num=filename.length();
        QStringList fileContent;
        QString str;
        QStringList str_prt;
        QStandardItemModel *data=new QStandardItemModel;
        int row=0;

        for(int i=0; i<file_num; ++i){
            QFile file(filename[i]);
            if(!file.open(QIODevice::ReadOnly)) continue;

            //TODO:: show the wrong messege

            //qDebug()<<filename[i];
            //qDebug()<<" "<<100/file_num*(i+1)<<"\n";
            emit text("loading "+filename[i]);
            double load_progress=100.0/file_num*(i+1);

            //deal with the file
            QTextStream fstream(&file);
            str=fstream.readLine();
            if(i==0){
                str_prt=str.split(',',QString::SkipEmptyParts);
                data->setHorizontalHeaderLabels(str_prt);
            }
            int colNum, j;
            QStandardItem *item;
            while (!fstream.atEnd())
            {
                str=fstream.readLine();
                str_prt=str.split(',',QString::SkipEmptyParts);
                colNum=str_prt.size();
                if(colNum<=1) continue;
                for(j=0; j<colNum; ++j){
                    item=new QStandardItem(str_prt.at(j));
                    data->setItem(row,j,item);
                }
                row++;
           }
            //model->setHorizontalHeaderLabels(str_prt);

            emit changed(int(load_progress));
            file.close();
        }
        emit text("loading finished!!!");
        sleep(3);
        emit change(-1);
        emit done(true);
        emit sendData(data);
        exit();
    }
};

#endif // MYTHREAD_H
