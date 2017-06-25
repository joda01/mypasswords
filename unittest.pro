#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T07:03:58
#
#-------------------------------------------------

QT       += widgets testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = tst_unittesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += src/tests/tst_unittesttest.cpp

include($$PWD/jopassdb.pri)

DEFINES += SRCDIR=\\\"$$PWD/\\\" \
           UNIT_TEST

