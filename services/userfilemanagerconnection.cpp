#include "userfilemanagerconnection.h"

UserFileManagerConnection::UserFileManagerConnection( QTcpSocket* client )
{
    this->client = client;
    this->client->setParent( nullptr );
    this->client->moveToThread( this );
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
        MediaFile* mediaFile = nullptr; // HACK TO AVOID THE POSSIBILITY OF SEGMENTATION FAULT
        mediaFile = user->takeFile( files[i] ); // UNSAFE
        if ( mediaFile == nullptr ) // HACK TO AVOID THE POSSIBILITY OF SEGMENTATION FAULT
        {
            this->client->write( "Server error: no media file found\n" );
            this->client->waitForBytesWritten();
            return;
        }

        QString mediaPath = mediaFile->getPath();
        QString scope = mediaFile->isPublic()? "public" : "private";
        QString indexPath = mediaPath.section ("/", 0, -2);

        QFile indexFile ( indexPath + QString( "/index.txt" ) );
        if ( indexFile.open( QIODevice::ReadOnly  | QIODevice::Text ) )
        {
            QStringList newIndex;
            bool error = true;
            while (!indexFile.atEnd())
            {
                    QString line = indexFile.readLine();
                    line.remove("\n");
                    qDebug() << line + " != " + mediaFile->getName() +  "$" + scope;
                    if (line != mediaFile->getName() +  "$" + scope)
                        newIndex.append( line );
                    else
                        error = false;
            }
            indexFile.close();
            if ( error )
            {
                this->client->write( "Server error: Unable to find media in the index\n" );
                this->client->waitForBytesWritten();
                user->addFile( mediaFile );
                return;
            }
            if ( indexFile.open( QIODevice::WriteOnly  | QIODevice::Text ) )
            {
                for ( int i = 0; i < newIndex.size(); i++ )
                {
                    indexFile.write( newIndex.at( i ).toUtf8() );
                    if ( i+1 < newIndex.size() )
                        indexFile.write( "\n" );
                }
                indexFile.close();
            }
            else
            {
                this->client->write( "Server error: Unable to update the index (write error)\n" );
                this->client->waitForBytesWritten();
                user->addFile( mediaFile );
                return;
            }
        }
        else
        {
            this->client->write( "Server error: Unable to update the index (read error)\n" );
            this->client->waitForBytesWritten();
            user->addFile( mediaFile );
            return;
        }

        QFile toRemove (mediaPath);
        if ( !toRemove.remove() )
        {
            if ( indexFile.open( QIODevice::Append  | QIODevice::Text ) )
            {
                indexFile.write( QString ( mediaFile->getName() +  "$" + scope ).toUtf8() );
                indexFile.close();
            }
            else
            {
                this->client->write( "Server error: Unable to remove media (index not restored)\n" );
                this->client->waitForBytesWritten();
                user->addFile( mediaFile );
                return;
            }

            this->client->write( "Server error: Unable to remove media (index restored)\n" );
            this->client->waitForBytesWritten();
            user->addFile( mediaFile );
            return;
        }

        user->setMemoryUsed( user->getMemoryUsed() - mediaFile->getSize() );
    }

    this->client->write( "Done\n" );
    this->client->waitForBytesWritten();
}

/*void UserFileManagerConnection::handleDeletingFiles()
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
        MediaFile* mediaFile = nullptr; // HACK TO AVOID THE POSSIBILITY OF SEGMENTATION FAULT
        mediaFile = user->takeFile( files[i] ); // UNSAFE
        if ( mediaFile == nullptr ) // HACK TO AVOID THE POSSIBILITY OF SEGMENTATION FAULT
        {
            this->client->write( "error\n" );
            this->client->waitForBytesWritten();
            break;
        }

        QString path = mediaFile->getPath();
        QString scope = mediaFile->isPublic()? "public" : "private";

        qDebug() << path << scope;

        QProcess process;
        QStringList tmp;
        tmp << path;
        process.execute( QString( "rm" ), tmp );

        QStringList pathList = path.split( "/" );

        path.clear();

        for( int i = 0; i < pathList.size(); i++ )
            path += pathList[i] + "/";

        QFile* file = new QFile( path + QString( "index.txt" ) );

        file->open( QIODevice::ReadWrite );

        QTextStream rm( file );
        QString newFile;
        QString line = rm.readLine();

        while( !rm.atEnd() )
        {
            if( line != ( mediaFile->getName() + QString( "$" + scope ) ) )
                newFile.append( line );

            line = rm.readLine();
        }
        file->close();
        file->remove();
        delete file;

        file = new QFile( path + QString( "index.txt" ) );

        file->open( QIODevice::WriteOnly );
        file->write( newFile.toUtf8() );
        file->close();

        delete file;

        user->setMemoryUsed( user->getMemoryUsed() - mediaFile->getSize() );
    }

    this->client->write( "Done\n" );
    this->client->waitForBytesWritten();
}*/

void UserFileManagerConnection::handleChangingScope()
{
    this->client->write( "[MUSIC=1, VIDEO=2, IMAGE=3\n" );
    this->client->waitForBytesWritten();

    this->client->waitForReadyRead( -1 );
    QString userChoice = this->client->readLine();
    userChoice.chop( 1 );

    int choice = userChoice.toInt();


    this->client->write( "files[END to stop]\n" );
    this->client->waitForBytesWritten();

    this->client->waitForReadyRead( -1 );
    QString element = this->client->readLine();
    element.chop( 1 );

    QStringList files;

    while( element != "END" )
    {
        files.push_back( element );

        this->client->waitForReadyRead( -1 );
        element = this->client->readLine();
        element.chop( 1 );
    }

    this->client->write( "I'm working....\n" );
    this->client->waitForBytesWritten();

    QMap<QString, MediaFile*>* mediaFileList;
    QString path = user->getUserDirectory();

    switch( choice )
    {
    case 1:
        mediaFileList = user->getMediaFiles( FileType::MUSIC );
        path += "Music/index.txt";
        break;
    case 2:
        mediaFileList = user->getMediaFiles( FileType::VIDEO );
        path += "Videos/index.txt";
        break;
    case 3:
        mediaFileList = user->getMediaFiles( FileType::IMAGE );
        path += "Images/index.txt";
        break;
    default:
        this->client->write( "error\n" );
        this->client->waitForBytesWritten( -1 );
        this->client->close();
        return;
    }

    QFile* file;

    QStringList fileStringList;

    for( int i = 0; i < files.size(); i++ )
    {
        MediaFile* currentFile;
        if( mediaFileList->contains( files[i] ) )
        {
            currentFile = mediaFileList->value( files[i] );
            fileStringList.push_back( QString( files[i] + "$" + ( currentFile->isPublic() ? "private" : "public" ) ) );
        }
        currentFile->set_Public( currentFile->isPublic() ? false : true );
    }

    qDebug() << "il path è " << path;
    file = new QFile( path );
    file->open( QIODevice::ReadOnly );
    QTextStream* in = new QTextStream( file );
    QString line;
    QString newFile;

    while( !in->atEnd() )
    {
        line = in->readLine();
        qDebug() << "linea corrente " << line;
        bool modified = false;
        for( int i = 0; i < fileStringList.size(); i++ )
        {
            qDebug() << "filestringlist di i è " << fileStringList[i];
            qDebug() << "filestringlist di i choppato è " << fileStringList[i].split( "$" )[0];
            qDebug() << "line splittato è " << line.split( "$" )[0];
            if( line.split( "$" )[0] == fileStringList[i].split( "$" )[0] )
            {
                qDebug() << "sono nell ' if e filestringlist di i è " << fileStringList[i];
                newFile = newFile.append( fileStringList[i] + QString( "\n" ) );
                modified = true;
            }
        }
        if( !modified )
            newFile = newFile.append( line + QString( "\n" ) );

    }
    qDebug() << "sto cazzo di file di merda è " << newFile;

    newFile.chop( 1 );
    file->close();
    delete file;
    delete in;

    QStringList parameters;
    parameters.push_back( path );

    QProcess rm;
    rm.execute( QString( "rm" ), parameters );

    QProcess touch;
    touch.execute( QString( "touch" ), parameters );

    file = new QFile( path );
    file->open( QIODevice::WriteOnly );
    file->write( newFile.toUtf8() );
    file->close();

    this->client->write( "Done\n" );
    this->client->waitForBytesWritten();


}

UserFileManagerConnection::~UserFileManagerConnection()
{
    delete this->client;
}

