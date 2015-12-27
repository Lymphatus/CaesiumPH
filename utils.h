#ifndef UTILS_H
#define UTILS_H

#include "usageinfo.h"

#include <QList>
#include <QStringList>
#include <QSize>
#include <QTemporaryDir>
#include <QElapsedTimer>
#include <QTreeWidgetItem>

#define MAX_COLUMNS 5

enum cexifs {
    EXIF_COPYRIGHT,
    EXIF_DATE,
    EXIF_COMMENTS
};

enum list_columns {
    COLUMNS_NAME = 0,
    COLUMNS_ORIGINAL_SIZE = 1,
    COLUMNS_NEW_SIZE = 2,
    COLUMNS_SAVED = 3,
    COLUMNS_PATH = 4
};

typedef struct var {
    int exif;
    QList<cexifs> importantExifs;
    int progressive;
    bool overwrite;
    int outMethodIndex;
    QString outMethodString;
} cparams;

extern QString inputFilter;
extern QString clfFilter;
extern QStringList inputFilterList;
extern QString versionString;
extern int versionNumber;
extern QString updateVersionTag;
extern int buildNumber;
extern long originalsSize, compressedSize; //Before and after bytes count
extern int compressedFiles; //Compressed files count
extern cparams params; //Important parameters
extern UsageInfo* uinfo;
extern QStringList osAndExtension;
extern QTemporaryDir tempDir;
extern QElapsedTimer timer;

QString toHumanSize(int);
double humanToDouble(QString);
QString getRatio(qint64, qint64);
char* QStringToChar(QString s);
QSize getScaledSizeWithRatio(QSize size, int square); //Image preview resize
double ratioToDouble(QString ratio);
bool isJPEG(char* path);
QString msToFormattedString(qint64);
bool haveSameRootFolder(QList<QTreeWidgetItem *> items);

#endif // UTILS_H
