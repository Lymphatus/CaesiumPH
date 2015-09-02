#ifndef NETWORKOPERATIONS_H
#define NETWORKOPERATIONS_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProgressDialog>

class NetworkOperations : public QObject
{
    Q_OBJECT
public:
    explicit NetworkOperations(QObject *parent = 0);

public slots:
    void uploadUsageStatistics();
    void uploadFinished(QNetworkReply*);
    void getCurrentBuild();
    void showUpdateDownloadProgress(qint64, qint64);
    void flushUpdate();

public:
    void checkForUpdates();
    void downloadUpdateRequest();

signals:
    void checkForUpdatesFinished(int);
    void updateDownloadFinished(QString);

private:
    QNetworkAccessManager* networkManager = new QNetworkAccessManager();
    QNetworkReply* updateReply;
    QNetworkReply* downloadUpdateReply;
    QString releaseURL;
    QProgressDialog* pDialog;
};

#endif // NETWORKOPERATIONS_H
