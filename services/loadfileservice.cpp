#include "loadfileservice.h"

using namespace services;

LoadFileService::LoadFileService()
{
    connections = new QList<LoadFileConnection*>;
}

void LoadFileService::run()
{
    this->serverSocket = new QTcpServer();
    bool error = this->serverSocket->listen( QHostAddress::Any, 80002 );
    if( !error )
    {
        qDebug() << "errore listen";
    }
    while( true )
    {
        this->serverSocket->waitForNewConnection( -1, 0 );

        this->connections->push_back( new LoadFileConnection( this->serverSocket->nextPendingConnection() ) );       
        this->connections->back()->start();
    }
}

LoadFileService::~LoadFileService()
{
    delete connections;
}

