#include "dlnaprocess.h"
#include "../usermanager.h"
#include <QProcess>
#include <QStringList>

using namespace services;
using namespace dlna;
using namespace users;

DLNAProcess::DLNAProcess( QTcpSocket* client )
{
    this->client = client;
}

void DLNAProcess::run()
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

    userName_ID[1].chop( 1 );

    if( !userManager->getUser( userName_ID[0] )->isOnLine( userName_ID[1].toInt() ) )
    {
        this->client->write( "refused\n" );
        this->client->waitForBytesWritten( -1 );
        this->client->close();
    }
    else
    {
        User* user = userManager->getUser( userName_ID[0] );

        QMap<QString, MediaFile*>* music = user->getMediaFiles( FileType::MUSIC );
        QMap<QString, MediaFile*>* videos = user->getMediaFiles( FileType::VIDEO );
        QMap<QString, MediaFile*>* images = user->getMediaFiles( FileType::IMAGE );

        this->client->write( "list of files\n" );
        this->client->waitForBytesWritten( -1 );

        this->client->waitForReadyRead();

        QStringList parameters;
        parameters << user->getUserDirectory() + "/DLNA/config.xml";

        QString line = client->readLine();
        line.chop( 1 );
        while( line != "END" )
        {
            QStringList tokens = line.split( "=" );

            if( tokens[0] == "m" )
                parameters << "-a " << ( music->value( tokens[1] )->getPath() );
            else if( tokens[0] == "v" )
                parameters << "-a " << ( videos->value( tokens[1] )->getPath() );
            else
                parameters << "-a " <<(  images->value( tokens[1] )->getPath() );

        }

        this->client->write( "ok\n" );
        this->client->waitForBytesWritten( -1 );

        QProcess mediaTombProcess;

        mediaTombProcess.start( "mediatomb", parameters );


    }

}


DLNAProcess::~DLNAProcess()
{
}

