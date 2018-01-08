#-------------------------------------------------
#
# Project created by QtCreator 2018-01-03T22:36:53
#
#-------------------------------------------------

QT       += core gui\
            charts


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server_gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -std=c++11


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        serverthread.cpp \
    pugixml.cpp \
    ParticleFilterTracker.cpp \
    hungarian.cpp \
    graph.cpp \
    FrameQueue.cpp \
    DataAssociation.cpp \
    CameraConfig.cpp \
    ServerUDP.cpp \
    qcustomplot.cpp


HEADERS += \
        mainwindow.h \
        serverthread.h \
    pugixml.hpp \
    pugiconfig.hpp \
    ParticleFilterTracker.h \
    hungarian.h \
    graph.h \
    FrameQueue.h \
    Frame.h \
    DataAssociation.h \
    CameraConfig.h \
    ServerUDP.h \
    qcustomplot.h

LIBS += `pkg-config opencv --libs` \
        -lboost_system \
        -lpthread \
        -lboost_serialization \
        -lboost_date_time

FORMS += \
        mainwindow.ui

RESOURCES += \
    resource.qrc
