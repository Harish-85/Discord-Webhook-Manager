#ifndef DISCORDMESSAGE_H
#define DISCORDMESSAGE_H

#include <QJsonObject>
#include <QNetworkReply>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>

class DiscordMessage{

    QNetworkAccessManager* manager;
public:

     DiscordMessage(){
         manager = new QNetworkAccessManager;
        }


    void SendMessage(QString webhook, QString msg){
        QUrl url(webhook);

        QNetworkRequest request(url);

        request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject json;
        json["content"] = msg;

        QJsonDocument doc( json);
        QByteArray data = doc.toJson();

        QNetworkReply* reply = manager->post(request, data);

        // 5. Handle the response (Cleanup)


    }
};

#endif // DISCORDMESSAGE_H
