#ifndef UTILS_H
#define UTILS_H

#include "usageinfo.h"

#include <QList>
#include <QStringList>
#include <QSize>

typedef struct var {
    int exif;
    int progressive;
    bool overwrite;
} cparams;

extern QString inputFilter;
extern QStringList inputFilterList;
extern QString versionString;
extern int versionNumber;
extern int buildNumber;
extern long originalsSize, compressedSize; //Before and after bytes count
extern cparams params; //Important parameters
extern UsageInfo* uinfo;

QString formatSize(int);
QString getRatio(qint64, qint64);
char* QStringToChar(QString s);
QSize getScaledSizeWithRatio(QSize size, int square); //Image preview resize
double ratioToDouble(QString ratio);

#endif // UTILS_H
