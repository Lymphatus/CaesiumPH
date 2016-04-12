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

SOURCES += src/main.cpp\
        src/caesiumph.cpp \
    src/aboutdialog.cpp \
    src/cimageinfo.cpp \
    src/lossless.cpp \
    src/utils.cpp \
    src/exif.cpp \
    src/preferencedialog.cpp \
    src/networkoperations.cpp \
    src/qdroptreewidget.cpp \
    src/cphlist.cpp

HEADERS  += src/caesiumph.h \
    src/aboutdialog.h \
    src/cimageinfo.h \
    src/lossless.h \
    src/utils.h \
    src/exif.h \
    src/preferencedialog.h \
    src/networkoperations.h \
    src/qdroptreewidget.h \
    src/ctreewidgetitem.h \
    src/cphlist.h

FORMS    += \
    src/aboutdialog.ui \
    src/preferencedialog.ui \
    src/caesiumph.ui

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
    TODO.md \
    qt.conf \
    AUTHORS.md

TRANSLATIONS = translations/caesiumph_en_US.ts \
    translations/caesiumph_it_IT.ts \
    translations/caesiumph_zh_CN.ts \
    translations/caesiumph_ja_JP.ts
