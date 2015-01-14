#include "userfilemanagerconnection.h"
#include <QStringList>
#include <QFile>

UserFileManagerConnection::UserFileManagerConnection( QTcpSocket* client )
{
    this->client = client;
}

void UserFileManagerConnection::run()
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
        this->user = userManager->getUser( userName_ID[0] );

        while (true)
        {
            this->client->write( "what do I have to do?[DELETE_FILE=1, CHANGE_SCOPE=2, FINISH=3]\n" );
            this->client->waitForBytesWritten( -1 );

            this->client->waitForReadyRead();
            QString answer = this->client->readLine();

            if ( answer == "3\n" )
            {
                qDebug() << "NO MORE JOBS";
                this->client->close();
                return;
            }
            else
            {
                int choice = answer.toInt();
                switch( choice )
                {
                case 1:
                    handleDeletingFiles();
                    break;
                case 2:
                    handleChangingScope();
                    break;
                default:
                    this->client->write( "error, bye\n" );
                    this->client->waitForBytesWritten( -1 );
                    this->client->close();
                    return;
                }
            }
        }
    }
}

void UserFileManagerConnection::handleDeletingFiles()
{
    this->client->write( "files[END to stop]\n" );
    this->client->waitForBytesWritten();

    this->client->waitForReadyRead( -1 );
    QString element = this->client->readLine();
    element.chop( 1 );

    QStringList files;

    while( element != "END" )
    {
        files += element;

        this->client->waitForReadyRead( -1 );
        element = this->client->readLine();
        element.chop( 1 );
    }

    this->client->write( "I'm working....\n" );
    this->client->waitForBytesWritten();

    for( int i = 0; i < files.size(); i++ )
    {
        MediaFile* mediaFile = user->takeFile( files[i] );

        QString path = mediaFile->getPath();
        QString scope = mediaFile->isPublic()? "public" : "private";

        QFile* file = new QFile( path );
        file->remove();

        delete file;

        QStringList pathList = path.split( "/" );

        path.clear();

        for( int i = 0; i < pathList.size(); i++ )
            path += pathList[i];

        file = new QFile( path + QString( "index.txt" ) );

        file->open( QIODevice::ReadWrite );

        QTextStream rm( file );
        QString newFile;
        QString line = rm.readLine();

        while( !rm.atEnd() )
        {
            if( ( line + QString( "$" + scope ) ) != mediaFile->getName() )
                newFile.append( line );

            line = rm.readLine();
        }

        file->write( newFile.toUtf8() );
        file->close();

        user->setMemoryUsed( user->getMemoryUsed() - mediaFile->getSize() );
    }

    this->client->write( "Done\n" );
    this->client->waitForBytesWritten();
}

void UserFileManagerConnection::handleChangingScope()
{
    this->client->write( "files[END to stop]\n" );
    this->client->waitForBytesWritten();

    this->client->waitForReadyRead( -1 );
    QString element = this->client->readLine();
    element.chop( 1 );

    QStringList files;

    while( element != "END" )
    {
        files += element;

        this->client->waitForReadyRead( -1 );
        element = this->client->readLine();
        element.chop( 1 );
    }

    this->client->write( "I'm working....\n" );
    this->client->waitForBytesWritten();

    QMap<QString, MediaFile*>* music = user->getMediaFiles( FileType::MUSIC );
    QMap<QString, MediaFile*>* videos = user->getMediaFiles( FileType::VIDEO );
    QMap<QString, MediaFile*>* images = user->getMediaFiles( FileType::IMAGE );

    QFile* file;

    QStringList musicList;
    QStringList videoList;
    QStringList imageList;

    for( int i = 0; i < files.size(); i++ )
    {
        MediaFile* currentFile;
        if( music->contains( files[i] ) )
        {
            currentFile = music->value( files[i] );
            musicList += files[i];
        }

        else if( videos->contains( files[i] ) )
        {
            currentFile = videos->value( files[i] );
            videoList += files[i];
        }

        else if( images->contains( files[i] ) )
        {
            currentFile = images->value( files[i] );
            imageList += files[i];
        }

        currentFile->set_Public( currentFile->isPublic() ? false : true );
    }

    QString path = user->getUserDirectory() + "Music";

    file = new QFile( path );
    file->open( QIODevice::ReadWrite );
    QTextStream* in = new QTextStream( file );
    QString line = in->readLine();
    QString newFile;

    while( !in->atEnd() )
    {
        for( int i = 0; i < musicList.size(); i++ )
        {
            QStringList split = line.split( "$" );
            if( split[0] == musicList[i] )
                newFile.append( split[0] + QString( "$" ) + QString( split[1] == "public" ? "private" : "public" + QString( "\n" ) ) );
        }
       newFile.append( line + QString( "\n" ) );
       line = in->readLine();
    }

    file->close();
    file->write( newFile.toUtf8() );

    delete file;
    delete in;

    path = user->getUserDirectory() + "Videos";
    file = new QFile( path );
    in = new QTextStream( file );

    newFile.clear();

    while( !in->atEnd() )
    {
        for( int i = 0; i < videoList.size(); i++ )
        {
            QStringList split = line.split( "$" );
            if( split[0] == musicList[i] )
                newFile.append( split[0] + QString( "$" ) + QString( split[1] == "public" ? "private" : "public" + QString( "\n" ) ) );
        }
       newFile.append( line + QString( "\n" ) );
       line = in->readLine();
    }

    file->close();
    file->write( newFile.toUtf8() );

    delete file;
    delete in;

    path = user->getUserDirectory() + "Images";
    file = new QFile( path );
    in = new QTextStream( file );

    newFile.clear();

    while( !in->atEnd() )
    {
        for( int i = 0; i < imageList.size(); i++ )
        {
            QStringList split = line.split( "$" );
            if( split[0] == imageList[i] )
                newFile.append( split[0] + QString( "$" ) + QString( split[1] == "public" ? "private" : "public" + QString( "\n" ) ) );
        }
       newFile.append( line + QString( "\n" ) );
       line = in->readLine();
    }

    file->close();
    file->write( newFile.toUtf8() );

    delete file;
    delete in;

    this->client->write( "Done\n" );
    this->client->waitForBytesWritten();


}

UserFileManagerConnection::~UserFileManagerConnection()
{
    delete this->client;
}

