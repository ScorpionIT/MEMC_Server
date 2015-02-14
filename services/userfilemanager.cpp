#include "userfilemanager.h"
#include "userfilemanagerprocess.h"

using namespace services;

UserFileManager::UserFileManager() : GenericService()
{
}

void UserFileManager::run()
{
    int port = 80008;
    bool error = this->serverSocket->listen( QHostAddress::Any, port );
    if( !error )
    {
        qDebug() << "errore listen";
    }

    while( true )
    {
        if ( !this->serverSocket->hasPendingConnections() )
            this->serverSocket->waitForNewConnection( -1, 0 );

        this->connections->push_back( new UserFileManagerProcess( this->serverSocket->nextPendingConnection() ) );
        //connect( this->connections.back(), &Connection::closed, this, &ConnectionManager::killThreadConnetion );
        this->connections->back()->start();
    }

}

