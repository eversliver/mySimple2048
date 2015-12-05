#-------------------------------------------------
#
# Project created by QtCreator 2015-12-05T11:55:22
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MySimple2048
TEMPLATE = app


SOURCES += main.cpp\
        windowwidget.cpp \
    gamewidget.cpp

HEADERS  += windowwidget.h \
    gamewidget.h

CONFIG += mobility
MOBILITY =
