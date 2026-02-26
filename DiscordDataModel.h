#ifndef DISCORDDATAMODEL_H
#define DISCORDDATAMODEL_H

#include <QAbstractItemModel>

class DiscordDataModel : public QAbstractItemModel{
    Q_OBJECT

    // QAbstractItemModel interface
    QVector<Server>* m_servers;
public:
    explicit DiscordDataModel(QVector<Server>* data, QObject* parent) : QAbstractListModel( parent), m_servers(data){}

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // DISCORDDATAMODEL_H
