#include "dlnaprocess.h"

using namespace services;
using namespace dlna;

DLNAProcess::DLNAProcess( QTcpSocket* client ) : GenericProcess( client )
{
    this->client->setParent( nullptr );
    this->client->moveToThread( this );
}

void DLNAProcess::run()
{
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

        this->user = userManager->getUser( userName_ID[0] );

        this->client->write( "[NEWSHARE = 1, SHARESTATUS=2]\n" );
        this->client->waitForBytesWritten( -1 );

        if (this->client->bytesAvailable() == 0)
            this->client->waitForReadyRead();

        QString answer = this->client->readLine();


        int choice = answer.toInt();
        switch( choice )
        {
        case 1:
            this->newDlnaShare();
            break;
        case 2:
            this->getCurrentDlnaShare();
            break;
        default:
            this->client->write( "Server error: unknow choice\n" );
            this->client->waitForBytesWritten( -1 );
        }

        this->client->close();
        return;
    }
}

bool DLNAProcess::setupDlna()
{
    QDir userDir ( this->user->getUserDirectory() );
    QDir dlnaDir ( this->user->getUserDirectory() + "/DLNA" );

    if ( dlnaDir.exists() )
        if ( !dlnaDir.removeRecursively() )
            return false;

    if ( userDir.mkpath( "DLNA/fakeweb" ) )
    {
        QFile configFile ( this->user->getUserDirectory() + "/DLNA/config.xml" );
        if ( configFile.open( QIODevice::WriteOnly ) )
        {
            configFile.write( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                              "<config version=\"2\">\n"
                              "  <server>\n"
                              "    <ui enabled=\"no\">\n"
                              "      <accounts enabled=\"no\"/>\n"
                              "    </ui>\n"
                              "  <udn/>\n"
                              "  <name>MEMC - " + this->user->getUserName().toUtf8() + "</name>\n"
                              "  <webroot>fakeweb</webroot>\n"
                              "  <pc-directory upnp-hide=\"yes\"/>\n"
                              "  <storage caching=\"yes\">\n"
                              "    <sqlite3 enabled=\"yes\">\n"
                              "      <database-file>mediaDB.db</database-file>\n"
                              "    </sqlite3>\n"
                              "    <mysql enabled=\"no\"/>\n"
                              "  </storage>\n"
                              "  </server>\n"
                              "  <import hidden-files=\"no\">"
                              "    <mappings>\n"
                              "      <extension-mimetype ignore-unknown=\"no\">\n"
                              "        <map from=\"mp3\" to=\"audio/mpeg\"/>\n"
                              "        <map from=\"ogg\" to=\"audio/ogg\"/>\n"
                              "        <map from=\"mkv\" to=\"video/x-matroska\"/>\n"
                              "        <map from=\"avi\" to=\"video/mpeg\"/>\n"
                              "      </extension-mimetype>\n"
                              "    </mappings>\n"
                              "  </import>\n"
                              "</config>");
            configFile.close();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void DLNAProcess::newDlnaShare()
{
    this->stopDlnaShare();
    if ( !this->setupDlna() )
    {
        this->client->write( "Server error: unable to setup DLNA\n" );
        this->client->waitForBytesWritten( -1 );
        return;
    }

    QDir dlnaHome( user->getUserDirectory( ) );
    QStringList parameters;
    parameters << "-m" << dlnaHome.absolutePath() << "-f" << "DLNA";

    QMap<QString, MediaFile*>* music = user->getMediaFiles( FileType::MUSIC );
    QMap<QString, MediaFile*>* videos = user->getMediaFiles( FileType::VIDEO );
    QMap<QString, MediaFile*>* images = user->getMediaFiles( FileType::IMAGE );

    QFile dlnaShared( user->getUserDirectory() + "/DLNA/dlna.shared" );
    if ( !dlnaShared.open( QIODevice::WriteOnly | QIODevice::Text) )
    {
        this->client->write( "Server error: unable to make index of shared file\n" );
        this->client->waitForBytesWritten( -1 );
        return;
    }

    this->client->write( "files[END to stop]\n" );
    this->client->waitForBytesWritten( -1 );

    if (this->client->bytesAvailable() == 0)
        this->client->waitForReadyRead();
    QString element = this->client->readLine();
    element.chop( 1 );

    if (element == "END")
    {
        this->client->write( "ok\n" );
        this->client->waitForBytesWritten( -1 );
        return;
    }

    while( element != "END" && !element.isEmpty() )
    {
        QStringList tokens = element.split( "=" );
        if ( tokens.count() == 2 )
        {
            parameters << "-a";
            if( tokens[0] == "m" )
                parameters << music->value( tokens[1] )->getPath();
            else if( tokens[0] == "v" )
                parameters <<  videos->value( tokens[1] )->getPath();
            else if( tokens[0] == "i" )
                parameters << images->value( tokens[1] )->getPath();
            else
            {
                this->client->write( "Server error: unknow file\n" );
                this->client->waitForBytesWritten( -1 );
                dlnaShared.close();
                return;
            }
            dlnaShared.write( element.toUtf8() + "\n" );
        }
        else
        {
            this->client->write( "Server error: protocol error\n" );
            this->client->waitForBytesWritten( -1 );
            dlnaShared.close();
            return;
        }

        if (this->client->bytesAvailable() == 0)
            this->client->waitForReadyRead();
        element = this->client->readLine();
        element.chop( 1 );
    }

    dlnaShared.close();

    if ( element.isEmpty() )
    {
        qDebug() << "connection lose";
        return;
    }

    QProcess *mediaTombProcess = new QProcess();
    mediaTombProcess->setProcessChannelMode(QProcess::MergedChannels);
    mediaTombProcess->setStandardOutputFile( dlnaHome.absolutePath() + "/DLNA/dlna.status" );
    mediaTombProcess->start( "mediatomb", parameters );

    if ( mediaTombProcess->waitForStarted() )
    {
        this->user->setDlnaProcess( mediaTombProcess );
        this->client->write( "ok\n" );
        this->client->waitForBytesWritten( -1 );
    }
    else
    {
        this->client->write( "Server error: unable to start mediatomb process\n" );
        this->client->waitForBytesWritten( -1 );
    }
}

void DLNAProcess::getCurrentDlnaShare()
{
    QProcess *mediaTombProcess = this->user->getDlnaProcess();
    if (mediaTombProcess == nullptr)
    {
        this->client->write( "not running\n" );
        this->client->waitForBytesWritten( -1 );
        return;
    }
    if (mediaTombProcess->state() != QProcess::Running)
    {
        this->client->write( "not running\n" );
        this->client->waitForBytesWritten( -1 );
        return;
    }

    QFile dlnaShared( user->getUserDirectory() + "/DLNA/dlna.shared" );
    if ( dlnaShared.open( QIODevice::ReadOnly | QIODevice::Text) )
    {
        while (!dlnaShared.atEnd())
        {
            QString line = dlnaShared.readLine();
            if ( !line.isEmpty() )
            {
                this->client->write( line.toUtf8() );
                this->client->waitForBytesWritten( -1 );
            }
        }
        dlnaShared.close();
    }
    else
    {
        this->client->write( "Server error: unable to get shared media, try to restart the condivision\n" );
        this->client->waitForBytesWritten( -1 );
        return;
    }
    this->client->write( "ok\n" );
    this->client->waitForBytesWritten( -1 );
}

void DLNAProcess::stopDlnaShare()
{
    QProcess *mediaTombProcess = this->user->getDlnaProcess();
    if (mediaTombProcess != nullptr)
    {
        this->user->setDlnaProcess( nullptr );
        mediaTombProcess->close();
        delete mediaTombProcess;
    }
}

void DLNAProcess::closeConnection()
{

}


DLNAProcess::~DLNAProcess()
{
    delete client;
    this->~GenericProcess();

}

