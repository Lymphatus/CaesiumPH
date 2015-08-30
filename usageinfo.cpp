#include "usageinfo.h"
#include "utils.h"

#include <QUuid>
#include <QDateTime>
#include <QSysInfo>
#include <QLocale>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDebug>


UsageInfo::UsageInfo() {

}

void UsageInfo::initialize() {
    jsonPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) +
            QDir::separator() +
            "caesium.ph";
    QFile jsonFile(jsonPath);
    qDebug() << jsonPath;
    if (jsonFile.exists()) {
        readJSON();
    } else {
        UUID = QUuid::createUuid().toString();
        timestamp = QDateTime::currentMSecsSinceEpoch();
        productName = QSysInfo::prettyProductName();
        locale = QLocale().bcp47Name();
        arch = QSysInfo::currentCpuArchitecture();
        appVersion = versionNumber;
        build = buildNumber;

        compressed_bytes = compressed_pictures = max_bytes = best_ratio = 0;
        writeJSON();
    }
}

void UsageInfo::readJSON() {
    QFile jsonFile(jsonPath);
    if (jsonFile.exists()) {
        jsonFile.open(QFile::ReadOnly);
        QByteArray data = jsonFile.readAll();
        QJsonDocument jsonD = QJsonDocument::fromBinaryData(data);
        QJsonObject json = jsonD.object();

        UUID = json["uuid"].toString();
        timestamp = json["timestamp"].toVariant().toULongLong();
        productName = json["productName"].toString();
        locale = json["locale"].toString();
        arch = json["arch"].toString();
        appVersion = json["appVersion"].toInt();
        build = json["build"].toInt();

        compressed_bytes = json["compressedBytes"].toVariant().toULongLong();
        compressed_pictures = json["compressedPictures"].toVariant().toUInt();
        max_bytes = json["maxBytes"].toVariant().toULongLong();
        best_ratio = json["bestRatio"].toDouble();
    } else {
        //TODO Change
        //qDebug() << "Cannot open JSON file";
    }
}

void UsageInfo::writeJSON() {
    QJsonObject json;
    json["uuid"] = UUID;
    json["timestamp"] = timestamp;
    json["productName"] = productName;
    json["locale"] = locale;
    json["arch"] = arch;
    json["appVersion"] = appVersion;
    json["build"] = build;

    json["compressedBytes"] = QJsonValue::fromVariant(QVariant(compressed_bytes));
    json["compressedPictures"] = QJsonValue::fromVariant(QVariant(compressed_pictures));
    json["maxBytes"] = QJsonValue::fromVariant(QVariant(max_bytes));
    json["bestRatio"] = best_ratio;

    QJsonDocument jsonD(json);
    QFile outFile(jsonPath);
    QDir().mkpath(QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation).at(0));
    outFile.open(QFile::WriteOnly);
    outFile.write(jsonD.toBinaryData());
}

QString UsageInfo::printJSON() {
    qDebug () << jsonPath;
    QFile jsonFile(jsonPath);
    if (jsonFile.exists()) {
        jsonFile.open(QFile::ReadOnly);
        QByteArray data = jsonFile.readAll();

        QJsonDocument jsonD = QJsonDocument::fromBinaryData(data);

        return QString(jsonD.toJson(QJsonDocument::Indented));
    }
}
void UsageInfo::setBest_ratio(double value)
{
    best_ratio = value;
}

void UsageInfo::setMax_bytes(unsigned int value)
{
    max_bytes = value;
}

void UsageInfo::setCompressed_pictures(unsigned int value)
{
    compressed_pictures = value;
}

void UsageInfo::setCompressed_bytes(unsigned long long value)
{
    compressed_bytes = value;
}


