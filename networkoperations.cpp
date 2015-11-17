#include "networkoperations.h"
#include "usageinfo.h"
#include "utils.h"


#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProgressDialog>
#include <QJsonDocument>

NetworkOperations::NetworkOperations(QObject *parent) : QObject(parent) {
    releaseURL = "https://github.com/Lymphatus/CaesiumPH/releases/download/v" +
            updateVersionTag + "/caesiumph-" + updateVersionTag +
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
    QNetworkRequest request;
    request.setUrl(QUrl("http://download.saerasoft.com/caesiumph/current"));
    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    updateReply = networkManager->get(request);
    connect(updateReply, SIGNAL(readyRead()), this, SLOT(getCurrentBuild()));
}

void NetworkOperations::getCurrentBuild() {
    //Actually gets the build number
    if (updateReply->error() == QNetworkReply::NoError) {
        int v_number = updateReply->readLine().split('\n').at(0).toInt();
        QString v_string = updateReply->readLine().replace("\n", "");
        emit checkForUpdatesFinished(v_number,
                                     v_string);
    } else {
        qDebug() << updateReply->errorString();
    }
    updateReply->close();
}

void NetworkOperations::downloadUpdateRequest() {

    //ProgressDialog for progress display
    pDialog = new QProgressDialog();
    pDialog->setWindowTitle(tr("CaesiumPH Updater"));
    pDialog->setLabelText(tr("Downloading updates..."));
    pDialog->setWindowModality(Qt::WindowModal);
    //Set the right URL according to OS
    QUrl url;
    url.setUrl(releaseURL);

    //Build a request and set an header
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );

    //Get request
    downloadUpdateReply = networkManager->get(request);
    qDebug() << "Get started";

    //Connections
    connect(downloadUpdateReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(showUpdateDownloadProgress(qint64,qint64)));
    connect(downloadUpdateReply, SIGNAL(finished()), pDialog, SLOT(close()));
    connect(pDialog, SIGNAL(canceled()), downloadUpdateReply, SLOT(abort()));
    connect(downloadUpdateReply, SIGNAL(finished()), this, SLOT(flushUpdate()));
}

void NetworkOperations::showUpdateDownloadProgress(qint64 c, qint64 t) {
    //Show the progress in the ProgressDialog
    if (downloadUpdateReply->error() == QNetworkReply::NoError) {
        if (downloadUpdateReply->attribute(QNetworkRequest::RedirectionTargetAttribute).isNull()) {
            pDialog->setRange(0, t);
            pDialog->setValue(c);
        } else {
            return;
        }
    } else {
        qDebug() << "Network error: " + downloadUpdateReply->errorString() << " " << releaseURL;
        pDialog->close();
        downloadUpdateReply->abort();
    }
}

void NetworkOperations::flushUpdate() {
    //TODO If it's cancelled it closes CaesiumPH anyways

    if (downloadUpdateReply->error() == QNetworkReply::NoError) {

        //Gets a temporary path where we can write
        QString tmpPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) +
                QDir::separator() +
                "cph_u" +
                osAndExtension.at(1);

        //Check if we are being redirected
        QVariant variant = downloadUpdateReply->attribute(QNetworkRequest::RedirectionTargetAttribute);

        if (!variant.isNull()) {
            //Handle redirection; abort the current operation
            downloadUpdateReply->abort();
            //Change the url to the new one
            releaseURL = variant.toString();
            qDebug() << releaseURL;
            //Go again
            NetworkOperations::downloadUpdateRequest();
            //Don't forget to return to abort the current function
            return;
        }

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
    } else if (downloadUpdateReply->error() == QNetworkReply::OperationCanceledError) {
        qDebug() << "Aborted";
    }
}
