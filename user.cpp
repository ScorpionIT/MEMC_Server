#include "user.h"

User::User()
{
    musicFiles = new QList<MediaFile*>();
    videoFiles = new QList<MediaFile*>();
    imageFiles = new QList<MediaFile*>();
}

User::User( QString id, unsigned long memorySpace, QString userDirectory, QString passwd )
{
    this->userName = id;
    this->totalMemorySpace = memorySpace;
    this->userDirectory = userDirectory;
    this->passwd = passwd;

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
        musicFiles->push_back( file );
        break;
    case VIDEO:
        videoFiles->push_back( file );
        break;
    case IMAGE:
        imageFiles->push_back( file );
        break;

    }
}

bool User::getOnLine() const
{
    return onLine;
}


QList<MediaFile*>* User::getMediaFiles( FileType type ) const
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

User::~User()
{

}

