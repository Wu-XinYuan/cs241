#include "startingWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QDir::setCurrent(a.applicationDirPath());
    StartingWindow w;
    w.show();
    return a.exec();
}
