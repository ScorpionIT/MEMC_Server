#include "loadfileservice.h"

using namespace services;

LoadFileService::LoadFileService() : GenericService()
{
}

void LoadFileService::run()
{
    bool error = this->serverSocket->listen( QHostAddress::Any, 80002 );
    if( !error )
    {
        qDebug() << "errore listen";
    }
    while( true )
    {
        if ( !this->serverSocket->hasPendingConnections() )
                this->serverSocket->waitForNewConnection( -1, 0 );

        this->connections->push_back( new LoadFileProcess( this->serverSocket->nextPendingConnection() ) );       
        this->connections->back()->start();
    }
}

LoadFileService::~LoadFileService()
{
}

