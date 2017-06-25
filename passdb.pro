#-------------------------------------------------
#
# Project created by QtCreator 2016-09-17T06:43:34
#
#-------------------------------------------------

QT       += gui websockets network #core
CONFIG += c++11 crypto #console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PassDB
TEMPLATE = app

include($$PWD/passdb.pri)

