QT       += core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    analysiswindow.cpp \
    dataDisplayWindow.cpp \
    gridschangedialog.cpp \
    main.cpp \
    retrieveorderwindow.cpp \
    startingWindow.cpp \
    timechangedialog.cpp \
    timechoicedialog.cpp

HEADERS += \
    MyThread.h \
    analysiswindow.h \
    dataDisplayWindow.h \
    drawChartThread.h \
    gridschangedialog.h \
    retrieveorderwindow.h \
    startingWindow.h \
    timechangedialog.h \
    timechoicedialog.h

FORMS += \
    analysiswindow.ui \
    dataDisplayWindow.ui \
    gridschangedialog.ui \
    retrieveorderwindow.ui \
    startingWindow.ui \
    timechangedialog.ui \
    timechoicedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    picture.qrc
