#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DiscordDataTypes.h"
#include "DiscordMessage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void PopulateWebhookDropDown();
private slots:

    void on_btn_send_clicked();

    void on_cb_targetServer_activated(int index);

    void on_btn_config_clicked();

private:
    Ui::MainWindow *ui;
    DiscordConfigData* data = nullptr;
    DiscordMessage dMessager;

    void PopulateChannelDropdown();

    void loadJson();
    void initializeUI();
    void updateStatus();
};
#endif // MAINWINDOW_H
