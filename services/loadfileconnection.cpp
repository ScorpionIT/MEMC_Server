#include "loadfileconnection.h"
#include "usermanager.h"
#include <QFile>
#include <QTextStream>

using namespace services;

LoadFileConnection::LoadFileConnection( QTcpSocket* client )
{
    this->client = client;
}

void LoadFileConnection::closeConnection()
{
    this->client->close();
}

void LoadFileConnection::run()
{
    this->client->write( "who?\n" );
    bool hasAnswered = this->client->waitForReadyRead();

    if( !hasAnswered )
    {
        this->client->close();
        return;
    }

    QString userData = this->client->readLine();
    QStringList userName_ID = userData.split( " ",  QString::SkipEmptyParts );

    UserManager* userManager = UserManager::getInstance();

    userName_ID[1].chop( 2 );

    if( !userManager->getUser( userName_ID[0] )->isOnLine( userName_ID[1].toInt() ) )
    {
        this->client->write( "refused\n" );
        this->client->waitForBytesWritten( -1 );
        this->client->close();
    }
    else
    {
        User* user = userManager->getUser( userName_ID[0] );

        this->client->write( "what do you have for me?[MUSIC=1, VIDEO=2, IMAGES=3]\n" );
        this->client->waitForBytesWritten( -1 );

        this->client->waitForReadyRead();
        QString answer = this->client->readLine();


        QString directory = user->getUserDirectory();

        int choice = answer.toInt();

        switch ( choice )
        {
        case 1:
            directory += QString( "Music" ) + QString( "/" );
            break;

        case 2:
            directory += QString( "Video" ) + QString( "/" );
            break;
        case 3:
            directory += QString( "Images" ) + QString( "/" );
            break;
        default:
            this->closeConnection();
            return;
        }

        qDebug() << directory;

        this->client->write( "name?\n" );
        this->client->waitForBytesWritten( -1 );

        this->client->waitForReadyRead();
        QString fileName = this->client->readLine();
        fileName.chop( 1 );

        QFile* file = new QFile( directory + fileName );

        this->client->write( "buffer length?\n" );
        this->client->waitForBytesWritten( -1 );

        this->client->waitForReadyRead();
        int bufferLength = QString( this->client->readLine() ).toInt();

        file->open( QIODevice::WriteOnly );
        QTextStream fileStream( file );

        int filePieces = 1;

        bool isTrasmitting = this->client->waitForReadyRead();
        QString buffer = this->client->read( bufferLength );

        while( buffer != "END\n" )
        {
            fileStream << buffer;

            this->client->write( QString::number( filePieces ).toUtf8() + "\n" );
            this->client->waitForBytesWritten( -1 );

            filePieces++;

            isTrasmitting = this->client->waitForReadyRead();
            buffer = this->client->read( bufferLength );

            if( !isTrasmitting )
            {
                file->remove();
                delete file;
                this->closeConnection();
            }
        }

        file->close();

        this->client->write( "ok\n" );
        this->client->waitForBytesWritten( -1 );

    }
}

LoadFileConnection::~LoadFileConnection()
{
    delete client;
}

