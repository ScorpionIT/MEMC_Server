#include "user.h"

//using namespace services;
//using namespace dlna;
using namespace users;

User::User()
{
    musicFiles = new QMap<QString, MediaFile*>();
    videoFiles = new QMap<QString, MediaFile*>();
    imageFiles = new QMap<QString, MediaFile*>();

    dlnaSharing = new QMap<QString, services::dlna::DLNAProcess*>();
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

unsigned long User::getTotalMemorySpace() const
{
    return totalMemorySpace;
}

void User::setTotalMemorySpace( unsigned long value )
{
    totalMemorySpace = value;
}

unsigned long User::getMemoryUsed() const
{
    return memoryUsed;
}

void User::setMemoryUsed( unsigned long value )
{
    memoryUsed = value;
}

void User::addFile( MediaFile* file )
{
    FileType type = file->getType();

    switch( type )
    {
    case MUSIC:
        musicFiles->insert( file->getName(), file );
        break;
    case VIDEO:
        videoFiles->insert( file->getName(), file );
        break;
    case IMAGE:
        imageFiles->insert( file->getName(), file );
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

MediaFile* User::takeFile( QString name )
{
    if( musicFiles->contains( name ) )
        return musicFiles->take( name );
    else if( videoFiles->contains( name ) )
        return videoFiles->take( name );
    else if( imageFiles->contains( name ) )
        return imageFiles->take( name );
}

void User::addDLNASharing( QString pid, services::dlna::DLNAProcess* sharing )
{
    dlnaSharing->insert( pid, sharing );
}

bool User::removeDLNASharing( QString pid )
{
    if( !dlnaSharing->contains( pid ) )
        return false;

    dlnaSharing->remove( pid );
    return true;
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

}

