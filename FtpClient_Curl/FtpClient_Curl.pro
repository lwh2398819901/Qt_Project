#-------------------------------------------------
#
# Project created by QtCreator 2021-07-24T16:03:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FtpClient_Curl
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
    ftpclientwindow.cpp \
    ftpfileinfo.cpp \
    main.cpp \
    treeitem.cpp \
    ftpmanager.cpp \
    publishFunc/src/publishfunc_cxx.cpp \
    publishFunc/src/publishfunc_dtk.cpp \
    publishFunc/src/publishfunc_qt.cpp

HEADERS += \
        ftpclientwindow.h \
    ftpfileinfo.h \
    publishFunc/publishfunc.h \
    treeitem.h \
    ftpmanager.h \
    publishFunc/include/publishfunc_cxx.h \
    publishFunc/include/publishfunc_dtk.h \
    publishFunc/include/publishfunc_qt.h

FORMS += \
        ftpclientwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
