#include "connectionmanager.h"

ConnectionManager::ConnectionManager()
{
    this->serverSocket = new QTcpServer();

}

void ConnectionManager::killThreadConnetion(Connection* conn)
{
    qDebug() << "slot";
    qDebug() << sender();
    qDebug() << conn;
    //this->clientConnections.removeOne( sender() );
}

void ConnectionManager::start()
{
    bool error = this->serverSocket->listen( QHostAddress::Any, 80000 );
    qDebug() << error;

    while( true )
    {
        this->serverSocket->waitForNewConnection( -1, 0 );

        this->clientConnections.push_back( new Connection( this->serverSocket->nextPendingConnection() ) );
        connect( this->clientConnections.back(), &Connection::closed, this, &ConnectionManager::killThreadConnetion );
        this->clientConnections.back()->start();

//        for( QList<Connection*>::iterator it = clientConnections.begin(); it != clientConnections.end(); it++ )
//            if( (*it)->isFinished() )
//                it = clientConnections.erase( it );

//        qDebug() << "ci sono " << clientConnections.size() << " thread attivi";

    }

}

void ConnectionManager::stop()
{

}
