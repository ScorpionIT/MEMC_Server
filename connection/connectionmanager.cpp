#include "connectionmanager.h"

using namespace connection;

ConnectionManager::ConnectionManager()
{
    this->announcingSocket = new QUdpSocket();
    this->announceTime = new QTimer( this );

    connect( this->announceTime, SIGNAL( timeout() ), this, SLOT( sendAnnunceMessage() ) );
    this->announceTime->start( 5000 );
}

ConnectionManager::~ConnectionManager()
{
    delete serverSocket;
    delete announcingSocket;
    delete announceTime;


    for( QList<Connection*>::iterator it = clientConnections.begin(); it != clientConnections.end(); it++ )
        delete ( *it );

}

void ConnectionManager::killThreadConnetion()
{
    this->clientConnections.removeOne( ( Connection* )sender() );
}

void ConnectionManager::sendAnnunceMessage()
{
   this->announcingSocket->writeDatagram("MEMC-SERVER", QHostAddress::Broadcast, 79000);
}

void ConnectionManager::run()
{
    int port = 80000;
    this->serverSocket = new QTcpServer();
    bool error = this->serverSocket->listen( QHostAddress::Any, port );
    if (!error)
    {
        qDebug() << "errore listen";
    }
    while( true )
    {
        if ( !this->serverSocket->hasPendingConnections() )
            this->serverSocket->waitForNewConnection( -1, 0 );

        this->clientConnections.push_back( new Connection( this->serverSocket->nextPendingConnection() ) );
        connect( this->clientConnections.back(), &Connection::closed, this, &ConnectionManager::killThreadConnetion );
        this->clientConnections.back()->start();
    }

}

