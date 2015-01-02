#include "connectionmanager.h"

ConnectionManager::ConnectionManager()
{
    this->serverSocket = new QTcpServer();

}

void ConnectionManager::start()
{
    bool error = this->serverSocket->listen( QHostAddress::Any, 80000 );
    qDebug() << error;

    while( true )
    {
        this->serverSocket->waitForNewConnection( -1, 0 );

        this->clientConnections.push_back( new Connection( this->serverSocket->nextPendingConnection() ) );
        this->clientConnections.back()->start();


    }

}

void ConnectionManager::stop()
{

}
