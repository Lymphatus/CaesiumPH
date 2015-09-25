#include "networkoperations.h"
#include "usageinfo.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProgressDialog>

NetworkOperations::NetworkOperations(QObject *parent) : QObject(parent) {
    releaseURL = "http://download.saerasoft.com/caesiumph/latest/caesiumph-" +
            versionString +
            osAndExtension.at(1);
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
    //Request current build from network
    //TODO Rewrite with GitHub API
    updateReply = networkManager->get(QNetworkRequest(QUrl("http://download.saerasoft.com/caesiumph/current")));
    connect(updateReply, SIGNAL(readyRead()), this, SLOT(getCurrentBuild()));
}

void NetworkOperations::getCurrentBuild() {
    //Actually gets the build number
    if (updateReply->error() == QNetworkReply::NoError) {
        emit checkForUpdatesFinished(updateReply->readAll().toInt());
    } else {
        qDebug() << updateReply->errorString();
    }
    updateReply->close();
}

void NetworkOperations::downloadUpdateRequest() {
    //ProgressDialog for progress display
    pDialog = new QProgressDialog();
    pDialog->setWindowTitle(tr("CaesiumPH"));
    pDialog->setLabelText(tr("Downloading updates..."));
    //Set the right URL according to OS
    QUrl url;
    url.setUrl(releaseURL);

    //Get request
    downloadUpdateReply = networkManager->get(QNetworkRequest(url));

    //Connections
    connect(downloadUpdateReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(showUpdateDownloadProgress(qint64,qint64)));
    connect(pDialog, SIGNAL(canceled()), downloadUpdateReply, SLOT(abort()));
    connect(downloadUpdateReply, SIGNAL(finished()), this, SLOT(flushUpdate()));
}

void NetworkOperations::showUpdateDownloadProgress(qint64 c, qint64 t) {
    //Show the progress in the ProgressDialog
    if (downloadUpdateReply->error() == QNetworkReply::NoError) {
        qDebug() << "Update found: " + releaseURL;
        pDialog->setRange(0, t);
        pDialog->setValue(c);
    } else {
        qDebug() << "Network error: " + downloadUpdateReply->errorString();
        pDialog->close();
        downloadUpdateReply->abort();
    }
}

void NetworkOperations::flushUpdate() {
    //Gets a temporary path where we can write
    QString tmpPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) +
            QDir::separator() +
            "cph_u" +
            osAndExtension.at(1);

    QFile *file = new QFile(tmpPath);

    //Flush the file
    if (file->open(QFile::WriteOnly)) {
        file->write(downloadUpdateReply->readAll());
        file->flush();
        file->close();
        emit updateDownloadFinished(tmpPath);
        downloadUpdateReply->deleteLater();
    } else {
        downloadUpdateReply->deleteLater();
        qDebug() << "Failed to write file";
    }
}
