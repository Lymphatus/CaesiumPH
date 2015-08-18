#ifndef UTILS_H
#define UTILS_H

#include <QList>
#include <QStringList>
#include <QSize>


extern QString inputFilter;
extern QStringList inputFilterList;
extern QString versionString;
extern int versionNumber;
extern int buildNumber;

QString formatSize(int);
QString getRatio(qint64, qint64);
char* QStringToChar(QString s);
QSize getScaledSizeWithRatio(QSize size, int square);

#endif // UTILS_H
