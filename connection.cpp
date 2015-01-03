#include "connection.h"
#include "usermanager.h"
#include "rsacipher.h"
#include <QDebug>
#include <QtGlobal>
#include <QTime>

const unsigned long Connection::SESSION_TIMER = 10000;

int Connection::computeSessionID()
{
    qsrand( QTime::currentTime().msec() );
    return ( qrand() % UserManager::getInstance()->getNumOfUsers() + 1 ) * ( qrand() % 50  + 2 );
}

Connection::Connection(QTcpSocket* client)
{
    client->setParent(nullptr);
    this->client = client;
    this->client->moveToThread(this);
}

void Connection::run()
{
    bool loginCompleted = false;
    QString user;
    QString answer;
    do
    {
        do
        {
            this->client->write( "who?\n");

            this->client->waitForReadyRead( -1 );
            user = this->client->readLine();
            user.chop( 2 );
            qDebug()<< user;

        }while( !UserManager::getInstance()->isAUser( user ) );

        this->client->write( "pls pw\n");

        this->client->waitForReadyRead( -1 );
        answer = this->client->readLine();

        answer.chop( 2 );

        int attempt = 0;
        while( !(loginCompleted = UserManager::getInstance()->getUser( user )->isPasswdCorrect( answer ) ) && attempt < 3 )
        {
            this->client->write( "try again\n" );
            this->client->waitForReadyRead( -1 );
            answer = this->client->readLine();
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

    bool close = false;
    do
    {

        this->client->write( "are you there?\n" );
        this->client->waitForBytesWritten( -1 );

        bool hasItAnswered = this->client->waitForReadyRead( SESSION_TIMER );

        if( hasItAnswered )
        {
            answer = this->client->readLine();
            answer.chop( 2 );
            qDebug() << answer;
            if ( answer == "halt" )
            {
                close = true;
                this->client->write( "cià\n" );
                this->client->waitForBytesWritten( -1 );

            }
        }
        else
        {
            close = true;
        }

    }while ( !close );

    UserManager::getInstance()->getUser( user )->setIsOnline( false );
    UserManager::getInstance()->getUser( user )->setSessionID( -1 );
    this->client->close();
    delete this->client;
    emit closed();
}
