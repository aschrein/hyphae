#-------------------------------------------------
#
# Project created by QtCreator 2018-10-27T12:49:47
#
#-------------------------------------------------

QT       += core gui widgets
QT += printsupport

TARGET = hyphae
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
        mainwindow.h \
    qcustomplot.h

FORMS += \
        mainwindow.ui

copydata.commands = $(COPY_DIR) \"$$shell_path($$PWD\\tests)\" \"$$shell_path($$OUT_PWD\\tests)\"
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
