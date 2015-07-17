#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T17:46:21
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CaesiumPH
TEMPLATE = app

macx:LIBS += -L/usr/lib/libjpeg-turbo/lib -ljpeg -lturbojpeg
win32:LIBS += -LC:\\libjpeg-turbo-gcc\\lib -ljpeg -lturbojpeg

macx:INCLUDEPATH += /usr/local/include
win32:INCLUDEPATH += C:\\libjpeg-turbo-gcc\\include

CONFIG += warn_off

SOURCES += main.cpp\
        caesiumph.cpp \
    aboutdialog.cpp \
    global.cpp \
    lossless.cpp

HEADERS  += caesiumph.h \
    aboutdialog.h \
    global.h \
    lossless.h

FORMS    += caesiumph.ui \
    aboutdialog.ui

RESOURCES += \
    icons.qrc

DISTFILES += \
    TODO
