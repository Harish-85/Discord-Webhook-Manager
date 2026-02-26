#ifndef DISCORDDATATYPES_H
#define DISCORDDATATYPES_H
#include <QMetaType>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QIODevice>
#include <QFile>

struct Webhook{
    QString  name;
    QString url;

    QJsonObject toJson() const {
        return QJsonObject{
            {"name", name},
            {"url", url}
        };
    }

};

struct Channel {
    QString name;
    QVector<Webhook> webhooks;

    QJsonObject toJson() const {
        QJsonArray webhookArray;
        for (const auto& wh : webhooks) {
            webhookArray.append(wh.toJson());
        }
        return QJsonObject{
            {"name", name},
            {"webhooks", webhookArray}
        };
    }
};

struct Server{
    QString name;
    QVector<Channel> channels;

    QJsonObject toJson() const {
        QJsonArray channelArray;
        for (const auto& ch : channels) {
            channelArray.append(ch.toJson());
        }
        return QJsonObject{
            {"server_name", name},
            {"channels", channelArray}
        };
    }
};

class DiscordConfigData{
public:
    QVector<Server> Servers;


    QJsonDocument toDocument() const {
        QJsonArray rootArray;
        for (const auto& srv : Servers) {
            rootArray.append(srv.toJson());
        }
        return QJsonDocument(rootArray);
    }

    // Helper to save directly to a file
    bool saveToFile(const QString& path) const {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) return false;

        file.write(toDocument().toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }

    void loadJson(QString path){
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly)){
            qDebug() << "Could not open file for reading";
            return;
        }

        QByteArray rawData = file.readAll();
        file.close();

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(rawData, &error);

        // 2. Check if the JSON is actually valid
        if (error.error != QJsonParseError::NoError) {
            qDebug() << "JSON Parse Error:" << error.errorString();
            return;
        }

        loadFromDocument( doc);
    }

    void loadFromDocument(const QJsonDocument& doc) {
        Servers.clear(); // Start fresh
        QJsonArray rootArray = doc.array();

        for (int i = 0; i < rootArray.size(); ++i) {
            QJsonObject sObj = rootArray[i].toObject();
            Server srv;
            srv.name = sObj["server_name"].toString();

            QJsonArray chanArray = sObj["channels"].toArray();
            for (int j = 0; j < chanArray.size(); ++j) {
                QJsonObject cObj = chanArray[j].toObject();
                Channel chan;
                chan.name = cObj["name"].toString();

                QJsonArray hookArray = cObj["webhooks"].toArray();
                for (int k = 0; k < hookArray.size(); ++k) {
                    QJsonObject hObj = hookArray[k].toObject();
                    Webhook hook;
                    hook.name = hObj["name"].toString();
                    hook.url = hObj["url"].toString();
                    chan.webhooks.append(hook);
                }
                srv.channels.append(chan);
            }
            Servers.append(srv);
        }
    }
};

Q_DECLARE_METATYPE(Webhook)
Q_DECLARE_METATYPE(Channel)
Q_DECLARE_METATYPE(Server)
Q_DECLARE_METATYPE(Webhook*)
Q_DECLARE_METATYPE(Channel*)
Q_DECLARE_METATYPE(Server*)
#endif // DISCORDDATATYPES_H
