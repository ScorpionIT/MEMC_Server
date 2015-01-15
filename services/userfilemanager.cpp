#include "userfilemanager.h"
#include "userfilemanagerconnection.h"

UserFileManager::UserFileManager()
{
    connections = new QList<UserFileManagerConnection*>();
}

void UserFileManager::run()
{
    this->serverSocket = new QTcpServer();
    bool error = this->serverSocket->listen( QHostAddress::Any, 80008 );
    if( !error )
    {
        qDebug() << "errore listen";
    }

    while( true )
    {
        this->serverSocket->waitForNewConnection( -1, 0 );

        this->connections->push_back( new UserFileManagerConnection( this->serverSocket->nextPendingConnection() ) );
        //connect( this->connections.back(), &Connection::closed, this, &ConnectionManager::killThreadConnetion );
        this->connections->back()->start();
    }

}

UserFileManager::~UserFileManager()
{

}

