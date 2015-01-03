#include "connection.h"
#include "usermanager.h"
#include <QDebug>

Connection::Connection(QTcpSocket* client)
{
    this->client = client;
}

void Connection::run()
{

    bool loginCompleted = false;
    QString user;
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


        this->client->write( "ok pls pw\n");

        this->client->waitForReadyRead( -1 );
        QString answer = this->client->readLine();
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



    QString message("FileTransfer=80001 ");
    message += "DLNA=80002 ";
    message += "STREAMING=80003\n";
    this->client->write( message.toUtf8() );
    bool error = this->client->waitForBytesWritten();
    qDebug() << error;
}
