#include "user.h"

using namespace users;

User::User()
{
    musicFiles = new QMap<QString, MediaFile*>();
    videoFiles = new QMap<QString, MediaFile*>();
    imageFiles = new QMap<QString, MediaFile*>();

    publicMusicFiles = new QMap<QString, MediaFile*>();
    publicVideoFiles = new QMap<QString, MediaFile*>();
    publicImageFiles = new QMap<QString, MediaFile*>();

    this->dlnaProcess = nullptr;
}

QString User::getUserName() const
{
    return userName;
}

void User::setUserName( const QString& value )
{
    userName = value;
}

void User::setPasswd( QString passwd )
{
    this->passwd = passwd;
}

qint64 User::getTotalMemorySpace() const
{
    return totalMemorySpace;
}

void User::setTotalMemorySpace( qint64 value )
{
    totalMemorySpace = value;
}

qint64 User::getMemoryUsed() const
{
    return memoryUsed;
}

void User::setMemoryUsed(qint64 value )
{
    memoryUsed = value;
}

void User::addFile( MediaFile* file )
{
    FileType type = file->getType();

    switch( type )
    {
    case MUSIC:
        if( !file->isPublic() )
            musicFiles->insert( file->getName(), file );
        else
        {
            musicFiles->insert( file->getName(), file );
            publicMusicFiles->insert( file->getName(), file );
        }
        break;
    case VIDEO:
        if( !file->isPublic() )
            videoFiles->insert( file->getName(), file );
        else
        {
            videoFiles->insert( file->getName(), file );
            publicVideoFiles->insert( file->getName(), file );
        }
        break;
    case IMAGE:
        if( !file->isPublic() )
            imageFiles->insert( file->getName(), file );
        else
        {
            imageFiles->insert( file->getName(), file );
            publicImageFiles->insert( file->getName(), file );
        }
        break;
    }
}

bool User::getOnLine() const
{
    return onLine;
}


QMap<QString, MediaFile*>* User::getMediaFiles( FileType type ) const
{
    switch( type )
    {
    case MUSIC:
        return musicFiles;

    case VIDEO:
        return videoFiles;

    case IMAGE:
        return imageFiles;

    default:
        return nullptr;
    }
}

QMap<QString, MediaFile*>* User::getPublicFiles( FileType type ) const
{
    switch( type )
    {
    case MUSIC:
        return publicMusicFiles;

    case VIDEO:
        return publicVideoFiles;

    case IMAGE:
        return publicImageFiles;

    default:
        return nullptr;
    }
}

MediaFile* User::takeFile( QString name )
{
    if( musicFiles->contains( name ) )
        return musicFiles->take( name );
    else if( videoFiles->contains( name ) )
        return videoFiles->take( name );
    else if( imageFiles->contains( name ) )
        return imageFiles->take( name );
    else
        return nullptr;
}

MediaFile* User::getFile( QString name )
{
    if( musicFiles->contains( name ) )
        return musicFiles->value(name);
    else if( videoFiles->contains( name ) )
        return videoFiles->value(name);
    else if( imageFiles->contains( name ) )
        return imageFiles->value(name);
    else
        return nullptr;
}

QProcess* User::getDlnaProcess()
{
    return dlnaProcess;
}

void User::setDlnaProcess(QProcess *dlnaProcess)
{
    this->dlnaProcess = dlnaProcess;
}

void User::connect( int sessionID )
{
    onLine = true;
    this->sessionID = sessionID;
}

void User::disconnect()
{
    onLine = false;
    this->sessionID = -1;
}

bool User::isOnLine( int sessionID ) const
{
    if( sessionID == -1 )
        return false;

    return ( this->onLine && this->sessionID == sessionID );
}

bool User::isPasswdCorrect( QString passwd )
{
    return passwd == this->passwd;
}

QString User::getUserDirectory() const
{
    return this->userDirectory;
}

void User::setUserDirectory( QString path )
{
    this->userDirectory = path;
}

User::~User()
{
    if (this->dlnaProcess != nullptr)
    {
        this->dlnaProcess->close();
        delete this->dlnaProcess;
        this->dlnaProcess = nullptr;
    }

}

