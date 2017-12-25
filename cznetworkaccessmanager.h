#ifndef CZNETWORKACCESSMANAGER_H
#define CZNETWORKACCESSMANAGER_H
#include <QNetworkAccessManager>

class CZNetworkAccessManager : public QNetworkAccessManager
{
public:
    CZNetworkAccessManager(QObject *parent = Q_NULLPTR);

    QNetworkReply *post(const QNetworkRequest &request, QIODevice *data);
    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *post(const QNetworkRequest &request, QHttpMultiPart *multiPart);

    QNetworkReply *get(const QNetworkRequest &request);
};

#endif // CZNETWORKACCESSMANAGER_H
