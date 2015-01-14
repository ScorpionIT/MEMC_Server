#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "connection.h"
#include "../user.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QVector>

namespace connection
{
    class ConnectionManager : public QThread
    {
        Q_OBJECT

    private:
        QTcpServer* serverSocket;
        QList<Connection*> clientConnections;
        void run();

    private slots:
        void killThreadConnetion();

    };
}
#endif // CONNECTIONMANAGER_H
