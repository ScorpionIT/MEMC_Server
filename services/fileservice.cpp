#include "fileservice.h"
#include "fileprocess.h"
#include <QString>
#include <string>
using namespace std;
#include <QStringList>

using namespace services;

static const int SESSION_TIMER = 500;

FileService::FileService() : GenericService()
{
}

void FileService::run()
{
    int port = 80001;
    bool error = this->serverSocket->listen( QHostAddress::Any, port );
    if (!error)
    {
        qDebug() << "errore listen";
    }
    while( true )
    {
        if ( !this->serverSocket->hasPendingConnections() )
            this->serverSocket->waitForNewConnection( -1, 0 );

        connections->push_back( new FileProcess( serverSocket->nextPendingConnection() ) );
        connections->back()->start();
    }

}

FileService::~FileService()
{
}
