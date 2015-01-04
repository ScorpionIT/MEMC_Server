#include "connection.h"
#include "usermanager.h"
#include <QDebug>
#include <QtGlobal>
#include <QTime>

const unsigned long Connection::SESSION_TIMER = 10000;

int Connection::computeSessionID()
{
    qsrand( QTime::currentTime().msec() );
    return ( qrand() % UserManager::getInstance()->getNumOfUsers() + 1 ) * ( qrand() % 50  + 2 );
}

void Connection::closeConnection()
{
    if( !user.isEmpty() )
    {
        UserManager::getInstance()->getUser( user )->setIsOnline( false );
        UserManager::getInstance()->getUser( user )->setSessionID( -1 );
    }
    this->client->write( "timeout, bye\n" );
    this->client->waitForBytesWritten( -1 );
    this->client->close();
    delete this->client;

    this->terminate();
    emit closed();
}

Connection::Connection(QTcpSocket* client)
{
    client->setParent( nullptr );
    this->client = client;
    this->client->moveToThread( this );
}

void Connection::run()
{
    bool loginCompleted = false;

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

            user.chop( 2 );
            qDebug()<< user;

        }while( !UserManager::getInstance()->isAUser( user ) );

        this->client->write( "pls pw\n");

        this->client->waitForReadyRead( SESSION_TIMER );
        answer = this->client->readLine();

        answer.chop( 2 );

        int attempt = 0;
        while( !(loginCompleted = UserManager::getInstance()->getUser( user )->isPasswdCorrect( answer ) ) && attempt < 3 )
        {
            this->client->write( "try again\n" );
            this->client->waitForReadyRead( SESSION_TIMER );
            answer = this->client->readLine();

            if( answer.isEmpty() )
            {
                this->closeConnection();
            }
            attempt++;
        }
    }while( !loginCompleted );

    this->client->write( "ok\n" );


    this->client->write( QString( "s.id=" + QString::number( computeSessionID() ) + "\n" ).toUtf8() );

    QString message("FileTransfer=80001 ");
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
            answer.chop( 2 );

            if ( answer == "halt" )
            {
                this->client->write( "cià\n" );
                this->client->waitForBytesWritten( -1 );
                this->closeConnection();
            }
        }
        else
        {
            this->closeConnection();
        }

        qDebug() << "afsgdfsga";
    }



}
