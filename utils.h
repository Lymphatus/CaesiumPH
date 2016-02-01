/**
 *
 * This file is part of CaesiumPH.
 *
 * CaesiumPH - A Caesium version featuring lossless JPEG optimization/compression
 * for photographers and webmasters.
 *
 * Copyright (C) 2016 - Matteo Paonessa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>
 *
 */

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
    COLUMN_NAME = 0,
    COLUMN_ORIGINAL_SIZE = 1,
    COLUMN_NEW_SIZE = 2,
    COLUMN_SAVED = 3,
    COLUMN_PATH = 4
};

typedef struct var {
    int exif;
    QList<cexifs> importantExifs;
    int progressive;
    bool overwrite;
    int outMethodIndex;
    QString outMethodString;
} cparams;

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
extern QString lastCPHListPath; //Path of the last list saved
extern QList<QLocale> locales;

QString toHumanSize(long);
double humanToDouble(QString);
QString getRatio(qint64, qint64);
char* QStringToChar(QString s);
QSize getScaledSizeWithRatio(QSize size, int square); //Image preview resize
double ratioToDouble(QString ratio);
bool isJPEG(char* path);
QString msToFormattedString(qint64);
bool haveSameRootFolder(QList<QTreeWidgetItem *> items);
QString toCapitalCase(const QString);
void loadLocales();

#endif // UTILS_H
