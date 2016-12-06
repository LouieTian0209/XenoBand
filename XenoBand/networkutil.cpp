#include "networkutil.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QNetworkAccessManager *networkManager = new QNetworkAccessManager();

NetworkUtil::NetworkUtil()
{
}

QString NetworkUtil::sendMessage(QString name, QString message) {
    // send message
    QUrl url;
    QUrl serviceUrl = QUrl("http://xenoband.com/chat_qt.php");
    QByteArray postData;
    postData.append("name=").append(name).append("&");
    postData.append("message=").append(message);

    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, "PPL");

    QNetworkReply *reply = networkManager->post(request, postData);
    QEventLoop eventLoop;
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return reply->readAll();
}
