#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "connection.h"
#include "../user.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include <QList>
#include <QVector>

namespace connection
{
    class ConnectionManager : public QThread
    {
        Q_OBJECT

    public:
        ConnectionManager();
        virtual ~ConnectionManager();

    private:
        QTcpServer *serverSocket;
        QUdpSocket *announcingSocket;
        QTimer *announceTime;
        QList<Connection*> clientConnections;
        void run();

    private slots:
        void killThreadConnetion();
        void sendAnnunceMessage();

    };
}
#endif // CONNECTIONMANAGER_H
