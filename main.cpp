#include "caesiumph.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CaesiumPH w;
    w.show();

    a.setAttribute(Qt::AA_UseHighDpiPixmaps);

    return a.exec();
}
