#include "utils.h"
#include "math.h"
#include <QIODevice>
#include <QDate>
#include <QDebug>

QString inputFilter =  QIODevice::tr("Image Files (*.jpg *.jpeg)");
QStringList inputFilterList = QStringList() << "*.jpg" << "*.jpeg";
QString versionString = "0.9.9 (BETA)";
int versionNumber = 199;
int buildNumber = QDate::currentDate().toString("yyyyMMdd").toInt();

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
    return QString::number(((float) ((original - compressed) * 100) / (float) original), 'f', 1) + "%";
}

char* QStringToChar(QString s) {
    char* c_str = (char*) malloc((s.length() + 1) * sizeof(char));
    QByteArray bArray = s.toLocal8Bit();
    strcpy(c_str, bArray.data());
    return c_str;
}

QSize getScaledSizeWithRatio(QSize size, int square) {
    int w = size.width();
    int h = size.height();

    double ratio = 0.0;

    //Check the biggest between the two and scale on that dimension
    if (w >= h) {
        ratio = w / (double) square;
    } else {
        ratio = h / (double) square;
    }

    return QSize((int) round(w / ratio), (int) h / ratio);
}
