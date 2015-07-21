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
    QFile qss(":/qss/style.qss");
    qss.open(QFile::ReadOnly);
    //Apply
    QString style(qss.readAll());
    a.setStyleSheet(style);

    a.setFont(QFont(":/font/Roboto-Regular.ttf"));
    return a.exec();
}
