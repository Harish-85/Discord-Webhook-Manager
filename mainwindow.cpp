#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug()<<"Initialzing UI";
    ui->setupUi(this);
    loadJson();
    initializeUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_send_clicked()
{
    //dMessager
    int serverIndex  =ui->cb_server->currentIndex() ;

    if(serverIndex <0) return;

    Server srv = data->Servers[serverIndex];

    int channelIndex = ui->cb_channel->currentIndex();

    if(channelIndex<0) return;

    Channel ch = srv.channels[channelIndex];

    int webhookIndex = ui->cb_targetWebhook->currentIndex();

    if(webhookIndex<0) return;

    Webhook wh = ch.webhooks[webhookIndex];

    dMessager.SendMessage( wh.url,ui->te_msgBox->toPlainText());
}


void MainWindow::on_cb_targetServer_activated(int index)
{
}

void MainWindow::initializeUI(){
    qDebug()<<"Initailing Server dropdown";
    QComboBox* serverDropDown = ui->cb_server;

    serverDropDown->clear();
    if(data == nullptr || data->Servers.count() == 0) return;

    for(int i = 0; i< data->Servers.count() ; i++){
        serverDropDown->addItem(data->Servers[i].name);
    }
    connect(serverDropDown,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
        PopulateChannelDropdown();
    });

    PopulateChannelDropdown();


}

void MainWindow::PopulateChannelDropdown(){

    updateStatus();
    qDebug()<<"Initailing Channel dropdown";
    int serverIndex  =ui->cb_server->currentIndex() ;
    QComboBox* channelDropDown= ui->cb_channel;
    channelDropDown->clear();

    if(serverIndex <0) return;



    Server srv = data->Servers[serverIndex];

    for( int i =0; i< srv.channels.count(); i++){
        channelDropDown->addItem(srv.channels[i].name);
    }

    connect(channelDropDown,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
        PopulateWebhookDropDown();
    });

    PopulateWebhookDropDown();
}

void MainWindow::loadJson()
{
    data = new DiscordConfigData();
    data->loadJson("test.json");
}



void MainWindow::PopulateWebhookDropDown(){

    updateStatus();
    qDebug()<<"Initailing webhook dropdown";
    int serverIndex  =ui->cb_server->currentIndex() ;

    QComboBox* webhookDropDown = ui->cb_targetWebhook;

    webhookDropDown->clear();

    if(serverIndex <0) return;

    Server srv = data->Servers[serverIndex];

    int channelIndex = ui->cb_channel->currentIndex();

    if(channelIndex<0) return;



    Channel ch = srv.channels[channelIndex];

    for( int i =0; i< ch.webhooks.count(); i++){
        ui->cb_targetWebhook->addItem(ch.webhooks[i].name);
    }

}

void MainWindow::on_btn_config_clicked()
{
    DiscordSettings discordSettingUI(nullptr,data);
    discordSettingUI.setModal(true);
    if(discordSettingUI.exec() == QDialog::Accepted){
        data = discordSettingUI.GetConfigData();
        initializeUI();
    }



}

void MainWindow::updateStatus(){

    QString msg = "";
    QComboBox* serverDropDown = ui->cb_server;
    msg ="Targetting Server: " + serverDropDown->currentText();

    QComboBox* channelDropDown = ui->cb_channel;
    msg += " | Channel : " + channelDropDown->currentText();


    QComboBox* webhook = ui->cb_targetWebhook;
    msg += " | Agent : " + webhook->currentText();

    statusBar()->showMessage( msg);

}
