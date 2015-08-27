#ifndef USAGEINFO_H
#define USAGEINFO_H

#include <QString>
#include <QTime>
#include <QStandardPaths>
#include <QDir>

class UsageInfo
{
public:
    UsageInfo();
    UsageInfo(QString jsonPath);

    void writeJSON();
    void readJSON();
    QString printJSON();

    QString UUID;
    qint64 timestamp;

    QString productName;
    QString locale;

    unsigned long long compressed_bytes;
    unsigned int compressed_pictures;
    qint64 max_bytes;
    double best_ratio;

    void setCompressed_bytes(unsigned long long value);
    void setCompressed_pictures(unsigned int value);
    void setMax_bytes(unsigned int value);
    void setBest_ratio(double value);

private:
    QString jsonPath = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation).at(0) +
            QDir::separator() +
            "usage.dat";
};

#endif // USAGEINFO_H
