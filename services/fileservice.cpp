#include "fileservice.h"
#include "usermanager.h"
#include <QString>
#include <QStringList>

using namespace services;

static const int SESSION_TIMER = 500;

FileService::FileService()
{

}

void FileService::run()
{
    this->serverSocket = new QTcpServer();
    bool error = this->serverSocket->listen( QHostAddress::Any, 80001 );
    if (!error)
    {
        qDebug() << "errore listen";
    }
    while( true )
    {
        this->serverSocket->waitForNewConnection( -1, 0 );
        this->client = this->serverSocket->nextPendingConnection();

        this->client->write( "who?\n" );
        bool hasAnswered = this->client->waitForReadyRead();

        if( !hasAnswered )
        {
            this->client->close();
            return;
        }

        QString user = this->client->readLine();
        QStringList userName_ID = user.split( " ",  QString::SkipEmptyParts );

        UserManager* userManager = UserManager::getInstance();

        userName_ID[1].chop( 2 );

        qDebug() << "il primo è " << userName_ID[0] << " il secondo invece " << userName_ID[1];

        if( !userManager->getUser( userName_ID[0] )->isOnLine( userName_ID[1].toInt() ) )
        {
            this->client->write( "refused\n" );
            this->client->waitForBytesWritten( -1 );
            this->client->close();
        }
        else
        {
            this->client->write( "ok\n" );
            this->client->waitForBytesWritten( -1 );

            this->client->write( "? [MUSIC=1, VIDEOS=2, IMAGES=3]\n" );
            this->client->waitForBytesWritten( -1 );

            this->client->waitForReadyRead();
            int choice = QString( this->client->readLine() ).toInt();

            QList<MediaFile*>* files;

            switch( choice )
            {
            case 1:
                files = userManager->getUser( userName_ID[0] )->getMediaFiles( FileType::MUSIC );
                break;
            case 2:
                files = userManager->getUser( userName_ID[0] )->getMediaFiles( FileType::VIDEO );
                break;
            case 3:
                files = userManager->getUser( userName_ID[0] )->getMediaFiles( FileType::IMAGE );
                break;

            default:
                this->client->write( "error\n" );
                this->client->waitForBytesWritten( -1 );
                this->client->close();
                continue;
            }

            if( files == nullptr || files->isEmpty() )
            {
                this->client->write( "no files\n" );
                this->client->waitForBytesWritten( -1 );
                this->client->close();
            }
            else
            {
                for( QList<MediaFile*>::iterator it = files->begin(); it != files->end(); it++ )
                {
                    this->client->write( (*it)->getName().toUtf8() + "\n" );
                    this->client->waitForBytesWritten( -1 );
                }
                this->client->write( "end\n" );
                this->client->waitForBytesWritten( -1 );
            }

            this->client->close();
        }


    }


}

FileService::~FileService()
{
    delete serverSocket;
    delete client;
}

