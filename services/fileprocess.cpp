#include "fileprocess.h"
#include "../usermanager.h"
using namespace users;
using namespace services;

FileProcess::FileProcess( QTcpSocket* client ) : GenericProcess( client )
{
    this->client->moveToThread( this );
}

void FileProcess::run()
{
    this->client->write( "who?\n" );
    bool hasAnswered = true;
    if (this->client->bytesAvailable() == 0)
        hasAnswered = this->client->waitForReadyRead();

    if( hasAnswered )
    {
        QString user = this->client->readLine();
        QStringList userName_ID = user.split( " ",  QString::SkipEmptyParts );

        UserManager* userManager = UserManager::getInstance();

        userName_ID[1].chop( 1 );

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

            if (this->client->bytesAvailable() == 0)
                this->client->waitForReadyRead();
            int choice = QString( this->client->readLine() ).toInt();

            QMap<QString, MediaFile*>* files;
            QList<MediaFile*>* publicFiles;

            switch( choice )
            {
            case 1:
                files = userManager->getUser( userName_ID[0] )->getMediaFiles( FileType::MUSIC );
                publicFiles = userManager->getPublicMediaFiles( user, MUSIC );
                break;
            case 2:
                files = userManager->getUser( userName_ID[0] )->getMediaFiles( FileType::VIDEO );
                publicFiles = userManager->getPublicMediaFiles( user, VIDEO );
                break;
            case 3:
                files = userManager->getUser( userName_ID[0] )->getMediaFiles( FileType::IMAGE );
                publicFiles = userManager->getPublicMediaFiles( user, IMAGE );
                break;

            default:
                this->client->write( "error\n" );
                this->client->waitForBytesWritten( -1 );
                this->client->close();
                return;
            }
            if( files == nullptr || files->isEmpty() )
            {
                this->client->write( "no files\n" );
                this->client->waitForBytesWritten( -1 );
                this->client->close();
            }
            else
            {
                this->client->write( "yours:\n" );
                this->client->waitForBytesWritten( -1 );
                for( QMap<QString, MediaFile*>::iterator it = files->begin(); it != files->end(); it++ )
                {
                    this->client->write( ( (*it)->getName().toUtf8() + QString( (*it)->isPublic() ? "$public" : "$private" ) + QString( "\n" ) ).toStdString().c_str() );
                    this->client->waitForBytesWritten( -1 );
                }
                this->client->write( "end\n" );
                this->client->write( "not yours(public):\n" );
                this->client->waitForBytesWritten( -1 );

                for( QList<MediaFile*>::iterator it = publicFiles->begin(); it != publicFiles->end(); it++ )
                {
                    this->client->write( ( (*it)->getName().toUtf8() + QString( "$public" ) + QString( "\n" ) ).toStdString().c_str() );
                    this->client->waitForBytesWritten( -1 );
                }
                this->client->write( "end\n" );
                this->client->waitForBytesWritten( -1 );
            }

            this->client->close();
        }
    }
    else
    {
        qDebug() << "connection lost";
        this->client->close();
    }
}

void FileProcess::closeConnection()
{

}



FileProcess::~FileProcess()
{

}

