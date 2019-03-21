#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T13:59:13
#
#-------------------------------------------------

QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IMUvisualizer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    processimudata.cpp \
    qglview.cpp

HEADERS  += mainwindow.h \
    processimudata.h \
    arrayoperations.h \
    qglview.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../AutonomyProtocol/release/ -lAutonomyProtocol
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../AutonomyProtocol/debug/ -lAutonomyProtocol
else:unix: LIBS += -L$$OUT_PWD/../../AutonomyProtocol/ -lAutonomyProtocol

INCLUDEPATH += $$PWD/../../AutonomyProtocol
DEPENDPATH += $$PWD/../../AutonomyProtocol

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../AutonomyProtocol/release/libAutonomyProtocol.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../AutonomyProtocol/debug/libAutonomyProtocol.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../AutonomyProtocol/release/AutonomyProtocol.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../AutonomyProtocol/debug/AutonomyProtocol.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../AutonomyProtocol/libAutonomyProtocol.a
