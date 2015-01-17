#include "userfilemanagerconnection.h"

UserFileManagerConnection::UserFileManagerConnection( QTcpSocket* client )
{
    this->client = client;
    this->client->setParent( nullptr );
    this->client->moveToThread( this );
}

UserFileManagerConnection::~UserFileManagerConnection()
{
    delete this->user;
    delete this->client;
}

void UserFileManagerConnection::run()
{
    this->client->write( "who?\n" );

    bool hasAnswered = true;
    if (!this->client->bytesAvailable())
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

        while (true)
        {
            if (!this->client->isOpen())
                return;

            this->client->write( "what do I have to do?[DELETE_FILE=1, LOCK_FILE=2, UNLOCK_FILE=3, FINISH=4]\n" );
            this->client->waitForBytesWritten( -1 );

            if (!this->client->bytesAvailable())
                this->client->waitForReadyRead();
            QString answer = this->client->readLine();

            if ( answer == "4\n" || answer.isEmpty() )
            {
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
                    this->toPublic = false;
                    handleChangingScope();
                    break;
                case 3:
                    this->toPublic = true;
                    handleChangingScope();
                    break;
                default:
                    this->client->write( "Server error: unknow choice\n" );
                    this->client->waitForBytesWritten( -1 );
                    this->client->close();
                    return;
                }
                this->client->close();
            }
        }
    }
}

void UserFileManagerConnection::handleDeletingFiles()
{
    this->client->write( "files[END to stop]\n" );
    this->client->waitForBytesWritten();

    if (!this->client->bytesAvailable())
        this->client->waitForReadyRead();
    QString element = this->client->readLine();
    element.chop( 1 );

    QStringList files;

    while( element != "END" )
    {
        files += element;

        if (!this->client->bytesAvailable())
            this->client->waitForReadyRead();
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

void UserFileManagerConnection::handleChangingScope()
{
    this->client->write( "[MUSIC=1, VIDEO=2, IMAGE=3]\n" );
    this->client->waitForBytesWritten();

    if (!this->client->bytesAvailable())
        this->client->waitForReadyRead();
    QString userChoice = this->client->readLine();
    userChoice.chop( 1 );

    int choice = userChoice.toInt();

    QString indexPath = user->getUserDirectory();
    switch( choice )
    {
    case 1:
        chageScopeMediaFileList = user->getMediaFiles( FileType::MUSIC );
        indexPath += "Music/index.txt";
        break;
    case 2:
        chageScopeMediaFileList = user->getMediaFiles( FileType::VIDEO );
        indexPath += "Videos/index.txt";
        break;
    case 3:
        chageScopeMediaFileList = user->getMediaFiles( FileType::IMAGE );
        indexPath += "Images/index.txt";
        break;
    default:
        this->client->write( "Server error: unknow media type\n" );
        this->client->waitForBytesWritten();
        return;
    }

    this->client->write( "files[END to stop]\n" );
    this->client->waitForBytesWritten();

    if (!this->client->bytesAvailable())
        this->client->waitForReadyRead();
    QString element = this->client->readLine();
    element.chop( 1 );

    while( element != "END" )
    {
        filesToChangeScope.push_back( element );

        if (!this->client->bytesAvailable())
            this->client->waitForReadyRead();
        element = this->client->readLine();
        element.chop( 1 );
        if ( element.isEmpty() )
        {
            qDebug() << "Connection lose";
            return;
        }
    }

    this->client->write( "I'm working....\n" );
    this->client->waitForBytesWritten();

    QStringList toChange;
    for( int i = 0; i < filesToChangeScope.size(); i++ )
    {
        if( chageScopeMediaFileList->contains( filesToChangeScope[i] ) )
        {
            toChange.append( filesToChangeScope[ i ] );
            chageScopeMediaFileList->value( filesToChangeScope[i] )->set_Public( this->toPublic );
        }
        else
        {
            this->client->write( "Server error: file not found\n" );
            this->client->waitForBytesWritten();
            this->restoreScope( i );
            return;
        }
    }

    QFile indexFile( indexPath );
    if ( indexFile.open( QIODevice::ReadOnly  | QIODevice::Text ) )
    {
        QStringList newIndex;
        int changed = 0;
        while (!indexFile.atEnd())
        {
            QString line = indexFile.readLine();
            line.remove("\n");
            QString fileName = line.section ("$", 0, -2);
            QString scope = line.section( "$", -1);
            if ( toChange.contains( fileName ) )
            {
                if ( scope == "public")
                    newIndex.append( fileName + "$private" );
                else if ( scope == "private")
                    newIndex.append( fileName + "$public" );
                else
                {
                    this->client->write( "Server error: Unable to find media in the index\n" );
                    this->client->waitForBytesWritten();
                    this->restoreScope();
                    indexFile.close();
                    return;
                }
                changed ++;
            }
            else
                newIndex.append( line );
        }
        indexFile.close();
        if ( changed <= 0 )
        {
            this->client->write( "Server error: Unable to find all media in the index\n" );
            this->client->waitForBytesWritten();
            this->restoreScope();
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
            this->restoreScope( );
            return;
        }
    }
    else
    {
        this->client->write( "Server error: Unable to update the index (read error)\n" );
        this->client->waitForBytesWritten();
        this->restoreScope();
        return;
    }

    this->client->write( "Done\n" );
    this->client->waitForBytesWritten();
}

void UserFileManagerConnection::restoreScope(int n)
{
    int end = filesToChangeScope.size();
    if ( n == -1 )
        end = n;

    for( int i = 0; i < end; i++ )
    {
        if( chageScopeMediaFileList->contains( filesToChangeScope[i] ) )
            chageScopeMediaFileList->value( filesToChangeScope[i] )->set_Public( this->toPublic );
    }
}
