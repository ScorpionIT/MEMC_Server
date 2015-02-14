#include "loadfileprocess.h"
#include "usermanager.h"
#include <QFile>
#include <QTextStream>

using namespace services;
using namespace users;

LoadFileProcess::LoadFileProcess( QTcpSocket* client ) : GenericProcess( client )
{
    this->client->moveToThread( this );
}

void LoadFileProcess::closeConnection()
{
    this->client->close();
}

void LoadFileProcess::run()
{
    if ( !this->client->isOpen() )
        return;

    this->client->write( "who?\n" );

    bool hasAnswered = true;
    if (this->client->bytesAvailable() == 0)
        hasAnswered = this->client->waitForReadyRead();

    if( !hasAnswered )
    {
        this->client->close();
        return;
    }

    QString userData = this->client->readLine();
    QStringList userName_ID = userData.split( " ",  QString::SkipEmptyParts );

    UserManager* userManager = UserManager::getInstance();

    userName_ID[1].chop ( 1 );

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
        User* user = userManager->getUser( userName_ID[0] );

        while (true)
        {
            if ( !this->client->isOpen() )
                return;

            this->client->write( "what do you have for me?[MUSIC=1, VIDEO=2, IMAGES=3, FINISH=4]\n" );
            this->client->waitForBytesWritten( -1 );

            if (this->client->bytesAvailable() == 0)
                this->client->waitForReadyRead();
            QString answer = this->client->readLine();

            if (answer == "4\n")
            {
                this->client->close();
                return;
            }

            QString directory = user->getUserDirectory();

            int choice = answer.toInt();

            switch ( choice )
            {
            case 1:
                directory += QString( "Music" ) + QString( "/" );
                break;

            case 2:
                directory += QString( "Videos" ) + QString( "/" );
                break;
            case 3:
                directory += QString( "Images" ) + QString( "/" );
                break;
            default:
                this->closeConnection();
                return;
            }

            this->client->write( "name?\n" );
            this->client->waitForBytesWritten( -1 );

            if (this->client->bytesAvailable() == 0)
                this->client->waitForReadyRead();
            QString fileName = this->client->readLine();
            fileName.chop( 1 );

            QFile* file = new QFile( directory + fileName );

            this->client->write( "buffer length?\n" );
            this->client->waitForBytesWritten( -1 );

            if (this->client->bytesAvailable() == 0)
                this->client->waitForReadyRead();
            int bufferLength = QString( this->client->readLine() ).toInt();

            file->open( QIODevice::WriteOnly );

            int filePieces = 1;

            bool isTrasmitting = true;
            if (this->client->bytesAvailable() == 0)
                isTrasmitting = this->client->waitForReadyRead();
            QByteArray buffer = this->client->read( bufferLength );

            while( QString( buffer ) != "END\n" && QString( buffer ) != "STOP\n" )
            {
                file->write( buffer );
                this->client->write( QString::number( filePieces ).toUtf8() + "\n" );
                this->client->waitForBytesWritten( -1 );

                filePieces++;

                isTrasmitting = true;
                if (this->client->bytesAvailable() == 0)
                    isTrasmitting = this->client->waitForReadyRead();
                buffer = this->client->read( bufferLength );

                if( !isTrasmitting )
                {
                    file->close();
                    file->remove();
                    this->closeConnection();
                }
            }
            if (QString( buffer ) == "STOP\n")
            {
                file->close();
                file->remove();
            }
            else
            {
                MediaFile* mediaFile = new MediaFile();

                mediaFile->setOwner( user->getUserName() );
                mediaFile->setSize( file->size() );
                mediaFile->setName( fileName );
                mediaFile->setPath( directory+fileName );
                mediaFile->setType( choice == 1 ? FileType::MUSIC : choice == 2 ? FileType::VIDEO : FileType::IMAGE );
                mediaFile->set_Public( false );

                if( file->size() + user->getMemoryUsed() <= user->getTotalMemorySpace() )
                {
                    user->setMemoryUsed( user->getMemoryUsed() + file->size() );
                    user->addFile( mediaFile );

                    QFile indexFile( directory + "index.txt" );

                    if ( indexFile.open(QIODevice::Append | QIODevice::Text ) )
                    {
                        indexFile.write( QString (fileName + "$private" + "$" + QString::number( file->size() ) + "\n").toUtf8() );
                        indexFile.close();
                    }
                    else
                    {
                        this->client->write( "Server error: Unable add media to index\n" );
                        this->client->waitForBytesWritten();
                        file->remove();
                    }
                }
            }
            file->close();
            delete file;

            this->client->write( "ok\n" );
            this->client->waitForBytesWritten( -1 );
        }
    }
}


