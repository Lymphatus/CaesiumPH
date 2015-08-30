#ifndef NETWORKOPERATIONS_H
#define NETWORKOPERATIONS_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetworkOperations : public QObject
{
    Q_OBJECT
public:
    explicit NetworkOperations(QObject *parent = 0);

public slots:
    void uploadUsageStatistics();
    void uploadFinished(QNetworkReply*);
    void getCurrentBuild();

public:
    void checkForUpdates();

signals:
    void checkForUpdatesFinished(int);

private:
    QNetworkAccessManager* networkManager = new QNetworkAccessManager();
    QNetworkReply* updateReply;
    QString releaseURL;
};

#endif // NETWORKOPERATIONS_H
