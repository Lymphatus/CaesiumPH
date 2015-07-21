#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T17:46:21
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CaesiumPH
TEMPLATE = app

macx:LIBS += -L/opt/mozjpeg/lib -ljpeg -lturbojpeg
win32:LIBS += -LC:\\libjpeg-turbo-gcc\\lib -ljpeg -lturbojpeg

macx:INCLUDEPATH += /opt/mozjpeg/include
win32:INCLUDEPATH += C:\\libjpeg-turbo-gcc\\include

macx {
    ICON = icons/icons/icon.icns
}

CONFIG += warn_off

SOURCES += main.cpp\
        caesiumph.cpp \
    aboutdialog.cpp \
    global.cpp \
    lossless.cpp \
    cimageinfo.cpp

HEADERS  += caesiumph.h \
    aboutdialog.h \
    global.h \
    lossless.h \
    cimageinfo.h

FORMS    += caesiumph.ui \
    aboutdialog.ui

RESOURCES += \
    icons.qrc \
    style.qrc

DISTFILES += \
    TODO \
    qss/style_osx.qss
