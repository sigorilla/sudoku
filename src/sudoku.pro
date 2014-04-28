#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T17:18:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sudoku
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    sudoku.cpp \
    settings.cpp

HEADERS  += mainwindow.h\
    ui_mainwindow_my.h \
    sudoku.h \
    settings.h

RESOURCES += files.qrc
