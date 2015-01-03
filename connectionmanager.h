#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "connection.h"
#include "user.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QVector>

class ConnectionManager : public QObject
{
    Q_OBJECT

private:
    QVector<User*> users;
    QTcpServer* serverSocket;
    QList<Connection*> clientConnections;

private slots:
    void killThreadConnetion(Connection*conn);

public:
    ConnectionManager();
    void start();
    void stop();
};

#endif // CONNECTIONMANAGER_H
