#-------------------------------------------------
#
# Project created by QtCreator 2015-10-21T10:02:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CellNet
TEMPLATE = app


SOURCES += main.cpp\
    View/mainwindow.cpp \
    View/mapscene.cpp \
    Model/reliefgenerator.cpp \
    Model/coveragecalculator.cpp \
    Model/genetic.cpp \
    Model/model.cpp \
    printer.cpp

HEADERS  += View/mainwindow.hpp \
    View/mapscene.hpp \
    Model/reliefgenerator.hpp \
    Model/coveragecalculator.hpp \
    Model/genetic.hpp \
    Model/model.hpp \
    printer.hpp
