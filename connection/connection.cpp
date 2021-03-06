#include "connection.h"
#include "usermanager.h"
#include <QDebug>
#include <QtGlobal>
#include <QTime>

using namespace connection;
using namespace users;

const unsigned long Connection::SESSION_TIMER = 10000;

Connection::Connection(QTcpSocket* client)
{
    client->setParent( nullptr );
    this->client = client;
    this->client->moveToThread( this );
}

int Connection::computeSessionID()
{
    qsrand( QTime::currentTime().msec() );
    return ( qrand() % UserManager::getInstance()->getNumOfUsers() + 1 ) * ( qrand() % 50  + 2 );
}

void Connection::closeConnection()
{
    if( !user.isEmpty() )
    {
        UserManager::getInstance()->getUser( user )->disconnect();
    }
    this->client->write( "timeout, bye\n" );
    this->client->waitForBytesWritten( -1 );
    this->client->close();
    delete this->client;
    UserManager::leaveLock();

    this->terminate();
    emit closed();
}


void Connection::run()
{
    bool loginCompleted = false;

    UserManager::takeLock();

    QString answer;
    do
    {
        do
        {
            this->client->write( "who?\n");
            this->client->waitForReadyRead( SESSION_TIMER );
            user = this->client->readLine();

            if( user.isEmpty() )
                this->closeConnection();

            user.chop( 1 );
        }while( !UserManager::getInstance()->isAUser( user ) );

        this->client->write( "pls pw\n");

        this->client->waitForReadyRead( SESSION_TIMER );
        answer = this->client->readLine();

        answer.chop( 1 );

        int attempts = 0;
        while( !(loginCompleted = UserManager::getInstance()->getUser( user )->isPasswdCorrect( answer ) ) && attempts < 3 )
        {
            this->client->write( "try again\n" );
            this->client->waitForReadyRead( SESSION_TIMER );
            answer = this->client->readLine();

            if( answer.isEmpty() )
            {
                this->closeConnection();
            }
            attempts++;
        }
    }while( !loginCompleted );

    this->client->write( "ok\n" );

    User* currentUser = UserManager::getInstance()->getUser( user );
    currentUser->connect( computeSessionID() );

    this->client->write( QString( "s.id=" + QString::number( currentUser->sessionID ) + "\n" ).toUtf8() );
    this->client->write( QString( "totmemory=" + QString::number( currentUser->getTotalMemorySpace() ) + " "
                                  + "usedmemory=" + QString::number( currentUser->getMemoryUsed() ) + "\n" ).toUtf8() );

    QString message( "FileTransfer=80001 ");
    message += "DLNA=80002 ";
    message += "STREAMING=80003\n";
    this->client->write( message.toUtf8() );
    this->client->waitForBytesWritten( -1 );
    bool hasItAnswered;

    while( true )
    {

        this->client->write( "are you there?\n" );
        this->client->waitForBytesWritten( -1 );

        hasItAnswered = this->client->waitForReadyRead( SESSION_TIMER );

        if( hasItAnswered )
        {
            answer = this->client->readLine();
            answer.chop( 1 );

            if ( answer == "halt" )
            {
                this->client->write( "bye\n" );
                this->client->waitForBytesWritten( -1 );
                this->closeConnection();
            }
        }
        else
        {
            this->closeConnection();
        }
        UserManager::leaveLock();

        sleep ( SESSION_TIMER );
    }
}

Connection::~Connection()
{
    delete client;
}
