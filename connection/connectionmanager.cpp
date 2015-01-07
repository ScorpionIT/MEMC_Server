#include "connectionmanager.h"

using namespace connection;

void ConnectionManager::killThreadConnetion()
{
    this->clientConnections.removeOne( ( Connection* )sender() );
}

void ConnectionManager::run()
{
    this->serverSocket = new QTcpServer();
    bool error = this->serverSocket->listen( QHostAddress::Any, 80000 );
    if (!error)
    {
        qDebug() << "errore listen";
    }
    while( true )
    {
        this->serverSocket->waitForNewConnection( -1, 0 );

        this->clientConnections.push_back( new Connection( this->serverSocket->nextPendingConnection() ) );
        connect( this->clientConnections.back(), &Connection::closed, this, &ConnectionManager::killThreadConnetion );
        this->clientConnections.back()->start();
    }

}
