#include "user.h"

User::User()
{
    mediaFiles = new QList<MediaFile*>();
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

void User::addFile(MediaFile* file)
{
    mediaFiles->push_back( file );
}

bool User::getOnLine() const
{
    return onLine;
}

void User::setSessionID( int sessionID )
{
    this->sessionID = sessionID;
}

void User::setIsOnline( bool online )
{
    this->onLine = online;
}



bool User::isOnLine() const
{
    return this->onLine;
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

