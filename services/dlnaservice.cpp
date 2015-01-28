#include "dlnaservice.h"

using namespace services;
using namespace dlna;

DLNAService::DLNAService() : GenericService()
{
}

void DLNAService::run()
{
    bool error = this->serverSocket->listen( QHostAddress::Any, 80007 );
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

DLNAService::~DLNAService()
{

}

