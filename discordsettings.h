#ifndef DISCORDSETTINGS_H
#define DISCORDSETTINGS_H

#include <QDialog>
#include <QTreeView>
#include <QStandardItemModel>
#include <QPushButton>
#include "DiscordDataTypes.h"

#include <QVariant>
#include <QVector>

namespace Ui {
class DiscordSettings;
}

class DiscordSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DiscordSettings(QWidget *parent = nullptr,DiscordConfigData* cfgData = nullptr);
    ~DiscordSettings();
    DiscordConfigData* data = nullptr;

    DiscordConfigData* GetConfigData();
private slots:
    void on_buttonBox_accepted();

private:
    Ui::DiscordSettings *ui;
    QStandardItemModel* treeData;
    QTreeView* treeView;
    QVector<Server*> servers;

    void SetupWebhookTree();
    void addChannel(QStandardItem* parentServer, QString name);
    QStandardItem* AddServer(QString name);

    void addWebHook(QStandardItem *parentChannel, QString name, QString url);
    void removeChannel(QStandardItem *item,QStandardItem *pserver);
    void LoadFromConfigJson(DiscordConfigData *data);
};

#endif // DISCORDSETTINGS_H
