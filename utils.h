#ifndef GLOBAL_H
#define GLOBAL_H

#include <QList>
#include <QStringList>
#include <QSize>


extern QString inputFilter;
extern QStringList inputFilterList;
extern QString versionString;
extern int buildNumber;

QString formatSize(int);
QString getRatio(qint64, qint64);
char* QStringToChar(QString s);
QSize getScaledSizeWithRatio(QSize size, int square);

#endif // GLOBAL_H
