#-------------------------------------------------
#
# Project created by QtCreator 2015-02-12T09:01:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tanks
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    world.cpp \
    tank.cpp \
    element.cpp

HEADERS  += mainwindow.h \
    world.h \
    tank.h \
    types.h \
    element.h

FORMS    += mainwindow.ui

LIBS += -lbox2d

QMAKE_CXXFLAGS += -std=c++0x
