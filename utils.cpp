#include "utils.h"
#include "math.h"
#include <stdlib.h>

#include <QIODevice>
#include <QDate>
#include <QDebug>

QString inputFilter =  QIODevice::tr("Image Files (*.jpg *.jpeg)");
QStringList inputFilterList = QStringList() << "*.jpg" << "*.jpeg";
QString versionString = "0.9.3-beta";
int versionNumber = 0;
int buildNumber = QDate::currentDate().toString("yyyyMMdd").toInt();
QString updateVersionTag = "";
long originalsSize = 0;
long compressedSize = 0;
int compressedFiles = 0;
cparams params;
UsageInfo* uinfo = new UsageInfo();
QStringList osAndExtension = QStringList() <<
        #ifdef _WIN32
            "win" << ".exe";
        #elif __APPLE__
            "osx" << ".dmg";
        #else
            "linux" << ".tar.gz";
        #endif
QTemporaryDir tempDir;
QElapsedTimer timer;

QString toHumanSize(int size) {
    double doubleSize = (double) size;
    if (size == 1) {
        return QString::number(size) + " bytes";
    } else if (size < 1024) {
        return QString::number(size) + " bytes";
    } else if (size >= 1024 && size < 1048576) {
        return QString::number(doubleSize / 1024, 'f', 2) + " Kb";
    } else if (size >= 1048576) {
        return QString::number(doubleSize / 1048576, 'f', 2) + " Mb";
    } else {
        return QString::number(size) + " bytes";
    }
}

double humanToDouble(QString human_size) {
    QStringList splitted = human_size.split(" ");
    if (splitted.at(1) == "Kb") {
        return QString(splitted.at(0)).toDouble() * 1024;
    } else if (splitted.at(1) == "Mb") {
        return QString(splitted.at(0)).toDouble() * 1048576;
    } else {
        return QString(splitted.at(0)).toDouble();
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

double ratioToDouble(QString ratio) {
    ratio = ratio.split("%").at(0);
    return ratio.toDouble();
}

bool isJPEG(char* path) {
    FILE* fp;
    unsigned char* type_buffer = (unsigned char*) malloc(2);

    fp = fopen(path, "r");

    if (fp == NULL) {
        fprintf(stderr, "Cannot open input file for type detection. Skipping.\n");
        return false;
    }

    if (fread(type_buffer, 1, 2, fp) < 2) {
        fprintf(stderr, "Cannot read file type. Skipping.\n");
        return false;
    }

    fclose(fp);

    if (((int) type_buffer[0] == 0xFF) && ((int) type_buffer[1] == 0xD8)) {
        free(type_buffer);
        return true;
    } else {
        fprintf(stderr, "Unsupported file type. Skipping.\n");
        return false;
    }
}

QString msToFormattedString(qint64 ms) {
    if (ms < 1000) {
        return QString::number(ms) + " ms";
    } else if (ms >= 1000 && ms < 60000) {
        return QString::number(((double) ms) / 1000, 'f', 1) + "s";
    } else {
        return QString::number(ms / 60000) + ":" + (ms < 70000 ? "0" : "") + QString::number(ms / 1000 % 60) + QT_TR_NOOP(" minutes");
    }
}
