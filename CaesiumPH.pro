#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T17:46:21
#
#-------------------------------------------------

QT       += core gui concurrent network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CaesiumPH
TEMPLATE = app

QMAKE_MAC_SDK = macosx10.11

macx {
    QMAKE_CXXFLAGS_CXX11 = -std=gnu++1y
    CONFIG *= c++11
    QMAKE_CXXFLAGS += -stdlib=libc++
    LIBS += -L/usr/local/lib -lexiv2.14 -L/opt/mozjpeg/lib -ljpeg.62 -stdlib=libc++
    INCLUDEPATH += /opt/mozjpeg/include /usr/local/include
    ICON = icons/icons/icon.icns
}

win32 {
    LIBS += -LC:\\mozjpeg\\lib -ljpeg -LC:\\exiv2\\src\\.libs -lexiv2
    INCLUDEPATH += C:\\mozjpeg\\include C:\\exiv2\\include
    RC_ICONS = icons/main/icon.ico
}

unix {
    LIBS += -ljpeg -lexiv2
}

CONFIG += warn_off c++11

SOURCES += main.cpp\
        caesiumph.cpp \
    aboutdialog.cpp \
    cimageinfo.cpp \
    lossless.cpp \
    utils.cpp \
    exif.cpp \
    preferencedialog.cpp \
    usageinfo.cpp \
    networkoperations.cpp \
    qdroptreewidget.cpp \
    ctreewidgetitem.cpp \
    cphlist.cpp

HEADERS  += caesiumph.h \
    aboutdialog.h \
    cimageinfo.h \
    lossless.h \
    utils.h \
    exif.h \
    preferencedialog.h \
    usageinfo.h \
    networkoperations.h \
    qdroptreewidget.h \
    ctreewidgetitem.h \
    cphlist.h

FORMS    += caesiumph.ui \
    aboutdialog.ui \
    preferencedialog.ui

RESOURCES += \
    icons.qrc \
    style.qrc

DISTFILES += \
    qss/style_osx.qss \
    qss/style_win.qss \
    qss/style_linux.qss \
    README.md \
    CHANGELOG.md \
    LICENSE \
    TODO.md

