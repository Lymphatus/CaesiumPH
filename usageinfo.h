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

#ifndef USAGEINFO_H
#define USAGEINFO_H

#include <QString>
#include <QTime>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>

#define USAGE_INFO_FILENAME QString("usageinfo.ph")

class UsageInfo
{

public:
    UsageInfo();

    QString jsonPath;

    void writeJSON();
    void readJSON();
    QString printJSON();
    void initialize();

    QString UUID;
    qint64 timestamp;

    QString productName;
    QString locale;
    QString arch;
    int appVersion;
    int build;

    unsigned long long compressed_bytes;
    unsigned int compressed_pictures;
    qint64 max_bytes;
    double best_ratio;


    void setCompressed_bytes(unsigned long long value);
    void setCompressed_pictures(unsigned int value);
    void setMax_bytes(unsigned int value);
    void setBest_ratio(double value);

private:

};

#endif // USAGEINFO_H
