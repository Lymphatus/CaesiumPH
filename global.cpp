#include "global.h"
#include <QIODevice>

QString inputFilter =  QIODevice::tr("Image Files (*.jpg *.jpeg)");
QStringList inputFilterList = QStringList() << "*.jpg" << "*.jpeg";
QString versionString = "1.9.9.0 (BETA)";
int buildNumber = 1990;

QString formatSize(int size) {
    double doubleSize = (double) size;
    if (size == 1) {
        return QString::number(size) + " bytes";
    } else if (size < 1024) {
        return QString::number(size) + " bytes";
    } else if (size >= 1024 && size < 1048576) {
        return QString::number(doubleSize / 1024, 'f', 1) + " Kb";
    } else if (size >= 1048576) {
        return QString::number(doubleSize / 1048576, 'f', 2) + " Mb";
    } else {
        return QString::number(size) + " bytes";
    }
}

QString getRatio(qint64 original, qint64 compressed) {
    float percentage = -((float) ((original - compressed) * 100) / (float) original);
    return QString::number(percentage, 'f', 1) + "%";
}

