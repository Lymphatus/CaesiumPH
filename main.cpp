#include "caesiumph.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CaesiumPH w;
    w.show();

    a.setAttribute(Qt::AA_UseHighDpiPixmaps);

    //Load style
    #ifdef __APPLE__
        QFile qss(":/qss/style_osx.qss");
    #elif _WIN32
        QFile qss(":/qss/style_win.qss");
    #else
        QFile qss(":/qss/style_unix.qss");
    #endif

    qss.open(QFile::ReadOnly);
    //Apply
    QString style(qss.readAll());
    a.setStyleSheet(style);

    //a.setFont(QFont(":/font/Roboto-Regular.ttf"));
    return a.exec();
}
