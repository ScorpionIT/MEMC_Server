#include "dlnaservice.h"
#include <QStringList>
#include <string>

DLNAService::DLNAService()
{
    processes = new QVector<DLNAProcess*>();
    this->serverSocket = new QTcpServer();
}

void DLNAService::start()
{
    bool error = this->serverSocket->listen( QHostAddress::Any, 80001 );

    while( true )
    {
        this->serverSocket->waitForNewConnection( -1, 0 );

        QTcpSocket* client = this->serverSocket->nextPendingConnection();
        QString message = client->readLine();

        QStringList files = message.split( " " );

        //this->processes->push_back( new DLNAProcess( files,  ) );
        this->processes->back()->start();

    }


}

DLNAService::~DLNAService()
{

}

