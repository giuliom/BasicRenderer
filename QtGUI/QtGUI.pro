#-------------------------------------------------
#
# Project created by QtCreator 2017-07-28T01:35:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGUI
TEMPLATE = app
RC_ICONS = Icon.ico
RC_FILE = QtGUI_resource.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    QRenderingWidget.cpp

HEADERS  += mainwindow.h\
		QRenderingWidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc