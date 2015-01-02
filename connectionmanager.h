#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "connection.h"
#include "user.h"
#include <QTcpServer>
#include <QTcpSocket>

class ConnectionManager
{
private:
    QVector<User*> users;
    QTcpServer* serverSocket;
    QVector<Connection*> clientConnections;

public:
    ConnectionManager();
    void start();
    void stop();
};

#endif // CONNECTIONMANAGER_H
