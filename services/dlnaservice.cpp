#include "dlnaservice.h"

using namespace services;
using namespace dlna;

DLNAService::DLNAService() : GenericService()
{
}

void DLNAService::run()
{
    int port = 80007;
    bool error = this->serverSocket->listen( QHostAddress::Any, port );
    if( !error )
    {
        qDebug() << "errore listen";
    }

    while( true )
    {
        if ( !this->serverSocket->hasPendingConnections() )
            this->serverSocket->waitForNewConnection( -1, 0 );

        this->connections->push_back( new DLNAProcess( this->serverSocket->nextPendingConnection() ) );
        //connect( this->connections.back(), &Connection::closed, this, &ConnectionManager::killThreadConnetion );
        this->connections->back()->start();
    }


}
