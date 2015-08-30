#include "networkoperations.h"
#include "usageinfo.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>

NetworkOperations::NetworkOperations(QObject *parent) : QObject(parent) {
    releaseURL = "http://download.saerasoft.com/caesiumph/" + os + "/current/";
}

void NetworkOperations::uploadUsageStatistics() {
    QString path = UsageInfo().jsonPath;
    QFile jsonFile(path);
    if (jsonFile.open(QFile::ReadOnly)) {
        QNetworkRequest request;
        QString finalUUID = uinfo->UUID.replace("{", "").replace("}", "");
        request.setUrl("http://saerasoft.com/pydio/ajaxplorer/shares/" + finalUUID);


        networkManager->put(request, jsonFile.readAll());
        connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(uploadFinished(QNetworkReply*)));
    }
}

void NetworkOperations::uploadFinished(QNetworkReply * reply) {
    qDebug() << reply->errorString();
}

void NetworkOperations::checkForUpdates() {
    //Get the current build
    updateReply = networkManager->get(QNetworkRequest(QUrl("http://download.saerasoft.com/caesiumph/current")));
    //TODO check for errors
    connect(updateReply, SIGNAL(readyRead()), this, SLOT(getCurrentBuild()));
}

void NetworkOperations::getCurrentBuild() {
    qDebug() << "Qui";
    if (updateReply->error() == QNetworkReply::NoError) {
        emit checkForUpdatesFinished(updateReply->readAll().toInt());
    } else {
        qDebug() << updateReply->errorString();
    }
}

