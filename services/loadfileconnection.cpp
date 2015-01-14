#include "loadfileconnection.h"
#include "usermanager.h"
#include <QFile>
#include <QTextStream>

using namespace services;
using namespace users;

LoadFileConnection::LoadFileConnection( QTcpSocket* client )
{
    this->client = client;
    this->client->setParent( nullptr );
    this->client->moveToThread( this );
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
            this->client->write( "what do you have for me?[MUSIC=1, VIDEO=2, IMAGES=3, FINISH=4]\n" );
            this->client->waitForBytesWritten( -1 );

            this->client->waitForReadyRead();
            QString answer = this->client->readLine();

            if (answer == "4\n")
            {
                qDebug() << "END FILE LIST";
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

            int filePieces = 1;

            bool isTrasmitting = this->client->waitForReadyRead();
            QByteArray buffer = this->client->read( bufferLength );

            while( QString( buffer ) != "END\n" && QString( buffer ) != "STOP\n" )
            {
                file->write( buffer );
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
            if (QString( buffer ) == "STOP\n")
                file->remove();

            file->close();


            this->client->write( "ok\n" );
            this->client->waitForBytesWritten( -1 );
            file->close();

            MediaFile* mediaFile = new MediaFile();

            mediaFile->setOwner( user->getUserName() );
            mediaFile->setSize( file->size() );
            mediaFile->setName( fileName );
            mediaFile->setPath( directory );
            mediaFile->setType( choice == 1 ? FileType::MUSIC : choice == 2 ? FileType::VIDEO : FileType::IMAGE );
            mediaFile->set_Public( false );

            user->addFile( mediaFile );

            delete file;

            file = new QFile( directory + "index.txt" );

            file->open( QIODevice::WriteOnly | QIODevice::Append );
            QTextStream out( file );

            out << "\n" + fileName + "$private";

            file->close();

            delete file;

        }
    }
}

LoadFileConnection::~LoadFileConnection()
{
    delete client;
}

