#include "discordsettings.h"
#include "ui_discordsettings.h"

DiscordSettings::DiscordSettings(QWidget *parent,DiscordConfigData* cfgData)
    : QDialog(parent)
    , ui(new Ui::DiscordSettings)
{
    ui->setupUi(this);
    data=cfgData;
    SetupWebhookTree();
}

DiscordSettings::~DiscordSettings()
{
    delete ui;
}

QStandardItem* DiscordSettings::AddServer(QString name)
{
    QList<QStandardItem*> rows;
    QStandardItem* nameItem = new QStandardItem(name);
    QStandardItem* urlItem = new QStandardItem("");
    QStandardItem* actionItem = new QStandardItem();
    QStandardItem* actionItem2 = new QStandardItem();

    urlItem->setEditable(true);

    rows << nameItem << urlItem  << actionItem << actionItem2;

    treeData->appendRow(rows);

    QPushButton* addButton = new QPushButton("+ Channel");

    connect(addButton, &QPushButton::clicked, [this,nameItem](){
        addChannel(nameItem, "NewChannel");
    } );

    treeView->setIndexWidget( actionItem->index(),addButton);

    QPushButton* addServerBtn = new QPushButton("+Server");

    connect(addServerBtn, &QPushButton::clicked, [this,nameItem](){
        AddServer("newServer");
    } );

    treeView->setIndexWidget( urlItem->index(),addServerBtn);

    QPushButton* removeButton = new QPushButton("delete");

    connect(removeButton ,&QPushButton::clicked, [this,nameItem](){
        if(treeData->rowCount() != 1)
            treeData->removeRow(nameItem->index().row());
    });

    treeView->setIndexWidget(actionItem2->index(),removeButton);

    return nameItem;
}

void DiscordSettings::addChannel(QStandardItem* parentServer, QString name){
    QList<QStandardItem*> row;
    QStandardItem* nameItem = new QStandardItem(name);
    QStandardItem* urlItem = new QStandardItem("");
    QStandardItem* actionItem = new QStandardItem();
    QStandardItem* actionItem2= new QStandardItem();

    row << nameItem << urlItem << actionItem <<actionItem2;
    parentServer->appendRow(row);


    QPushButton* btn = new QPushButton("+ Webhook");
    connect(btn, &QPushButton::clicked, [this, nameItem](){ addWebHook(nameItem, "New Hook", "https://..."); });
    treeView->setIndexWidget(actionItem->index(), btn);


    QPushButton* removeButton = new QPushButton("delete");

    connect(removeButton ,&QPushButton::clicked, [this,nameItem,parentServer](){
        removeChannel(nameItem, parentServer);
    });

    treeView->setIndexWidget(actionItem2->index(),removeButton);
    treeView->expand( parentServer->index());
}

void DiscordSettings::removeChannel(QStandardItem *item,QStandardItem *pserver)
{
    pserver->removeRow(item->index().row());

}

void DiscordSettings::addWebHook(QStandardItem* parentChannel, QString name, QString url=""){
    QList<QStandardItem*> row;
    QStandardItem* nameItem = new QStandardItem(name);
    QStandardItem* urlItem = new QStandardItem(url);
    QStandardItem* actionItem = new QStandardItem(); // No button for webhooks
    QStandardItem* actionItem2 = new QStandardItem();

    row << nameItem << urlItem << actionItem << actionItem2;

    parentChannel->appendRow(row);

    QPushButton* removeButton = new QPushButton("delete");

    connect(removeButton ,&QPushButton::clicked, [this,nameItem,parentChannel](){
        removeChannel(nameItem, parentChannel);
    });

    treeView->setIndexWidget(actionItem2->index(),removeButton);
    treeView->expand( parentChannel->index());
}

void DiscordSettings::SetupWebhookTree(){
    treeView = ui->webhookTree;


    treeData = new QStandardItemModel( 0,2,this);

    treeView->setModel(treeData);
    if(data != nullptr && data->Servers.count()!=0)
        LoadFromConfigJson(data);
    else
        AddServer("Main");
    treeView->expandAll();
    // Set this AFTER you have loaded your data into the model
    // Column 0: Name (Stretches to fill space)


    // Column 1: URL (Manual width)
    treeView->setColumnWidth(0, 300);
    treeView->setColumnWidth(1, 300); // 300 pixels wide

    treeView->setColumnWidth(2, 100);

    treeView->setColumnWidth(3, 80);
}

void DiscordSettings::LoadFromConfigJson(DiscordConfigData* data){
    treeData->clear();

    treeData->setHorizontalHeaderLabels({"Name", "URL", "Action", "Remove"});

    for (const auto& server : data->Servers) {
        QStandardItem* serverItem = AddServer(server.name);

        for (const auto& channel : server.channels) {
            addChannel(serverItem, channel.name);

            QStandardItem* channelItem = serverItem->child(serverItem->rowCount() - 1, 0);

            // 4. Loop through Webhooks for this channel
            for (const auto& webhook : channel.webhooks) {
                // Use your existing addWebHook logic
                addWebHook(channelItem, webhook.name, webhook.url);
            }
        }
    }

}

DiscordConfigData* DiscordSettings::GetConfigData(){

    DiscordConfigData* data = new DiscordConfigData();

    for( int i = 0; i< treeData->rowCount();i++){
        QStandardItem* serverItem = treeData->item(i);
        Server srv;
        srv.name = serverItem->text();

        for( int j = 0; j<serverItem->rowCount();j++){
            QStandardItem* channelItem = serverItem->child(j, 0);
            Channel ch;
            ch.name = channelItem->text();

            for (int k = 0; k < channelItem->rowCount(); ++k) {
                Webhook hw;
                hw.name = channelItem->child(k, 0)->text();

                QStandardItem* urlItem = channelItem->child(k, 1);
                hw.url = urlItem ? urlItem->text() : "";

                ch.webhooks.append(hw);
            }
            srv.channels.append(ch);
        }
        data->Servers.append(srv);
    }
    return data;
}




void DiscordSettings::on_buttonBox_accepted()
{
    GetConfigData()->saveToFile("test.json");
}

