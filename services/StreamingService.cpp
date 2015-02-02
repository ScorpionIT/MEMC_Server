#include "StreamingService.h"

using namespace services;


StreamingService::StreamingService()
{

}

void StreamingService::run()
{
    int port = 8086;
    bool error = this->serverSocket->listen( QHostAddress::Any, port );
    if( !error )
    {
        qDebug() << "errore listen";
    }

    while( true )
    {
        if ( !this->serverSocket->hasPendingConnections() )
            this->serverSocket->waitForNewConnection( -1, 0 );
        this->connections->push_back( new StreamingProcess( this->serverSocket->nextPendingConnection() ) );
        //connect( this->connections.back(), &Connection::closed, this, &ConnectionManager::killThreadConnetion );
        this->connections->back()->start();
    }
}
