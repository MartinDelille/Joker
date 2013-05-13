#-------------------------------------------------
#
# Project created by QtCreator 2013-04-25T11:26:46
#
#-------------------------------------------------

QT       += core gui
QT		 += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGLTest
TEMPLATE = app


SOURCES += main.cpp\
    phglwidget.cpp \
    testphglwidget.cpp

HEADERS  += \
    phglwidget.h \
    testphglwidget.h

FORMS    += mainwindow.ui

INCLUDEPATH += /Library/Frameworks
DEPENDPATH += /Library/Frameworks


