#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T17:46:21
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CaesiumPH
TEMPLATE = app

macx {
    QMAKE_CXXFLAGS_CXX11 = -std=gnu++1y
    CONFIG *= c++11
    QMAKE_CXXFLAGS += -stdlib=libc++
    LIBS += -stdlib=libc++
}

macx {
    LIBS += -L/usr/local/lib -lexiv2.14 -L/opt/mozjpeg/lib -ljpeg.62
}
win32 {
    LIBS += -LC:\\mozjpeg\\lib -ljpeg
    LIBS += -LC:\\exiv2\\src\\.libs -lexiv2
}

macx {
    INCLUDEPATH += /opt/mozjpeg/include /usr/local/include
}
win32 {
    INCLUDEPATH += C:\\mozjpeg\\include
    INCLUDEPATH += C:\\exiv2\\include
}

macx {
    ICON = icons/icons/icon.icns
}

CONFIG += warn_off

SOURCES += main.cpp\
        caesiumph.cpp \
    aboutdialog.cpp \
    cimageinfo.cpp \
    lossless.cpp \
    utils.cpp \
    exif.cpp

HEADERS  += caesiumph.h \
    aboutdialog.h \
    cimageinfo.h \
    lossless.h \
    utils.h \
    exif.h

FORMS    += caesiumph.ui \
    aboutdialog.ui

RESOURCES += \
    icons.qrc \
    style.qrc

DISTFILES += \
    TODO \
    qss/style_osx.qss \
    qss/style_win.qss \
    qss/style_linux.qss

