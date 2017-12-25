#include "cznetworkaccessmanager.h"

#include <QEventLoop>

CZNetworkAccessManager::CZNetworkAccessManager(QObject *parent)
    :QNetworkAccessManager(parent)
{

}

QNetworkReply *CZNetworkAccessManager::post(const QNetworkRequest &request, QIODevice *data)
{
    QEventLoop loop;
    connect(this, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkReply *reply = QNetworkAccessManager::post(request, data);
    loop.exec();
    return reply;
}

QNetworkReply *CZNetworkAccessManager::post(const QNetworkRequest &request, const QByteArray &data)
{
    QEventLoop loop;
    connect(this, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkReply *reply = QNetworkAccessManager::post(request, data);
    loop.exec();
    return reply;
}

QNetworkReply *CZNetworkAccessManager::post(const QNetworkRequest &request, QHttpMultiPart *multiPart)
{
    QEventLoop loop;
    connect(this, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkReply *reply = QNetworkAccessManager::post(request, multiPart);
    loop.exec();
    return reply;
}

QNetworkReply *CZNetworkAccessManager::get(const QNetworkRequest &request)
{
    QEventLoop loop;
    connect(this, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkReply *reply = QNetworkAccessManager::get(request);
    loop.exec();
    return reply;
}
