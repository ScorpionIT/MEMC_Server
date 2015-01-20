#include "userbuilder.h"
#include <QStringList>
using namespace UserManagerBuilding;

UserBuilder::UserBuilder()
{

}

void UserBuilder::setEntryPoint( QString entryPoint )
{
    this->entryPoint = entryPoint;
}

void UserBuilder::createUser( QString username )
{
    this->currentUser = new User();

    currentUser->setUserName( username );
    currentUser->setUserDirectory( entryPoint + username + "/");
    currentUser->setMemoryUsed( 0 );
}

void UserBuilder::createPasswd( QString passwd )
{
    currentUser->setPasswd( passwd );
}

void UserBuilder::addFile( QString file )
{
    MediaFile* newFile = new MediaFile();

    QStringList tokens = file.split( "$" );

    QString path = entryPoint + currentUser->getUserName() + "/" + tokens[0];

    newFile->setPath( path );
    newFile->setSize( tokens[2].toLong() );

    if( tokens[1] == "public" )
        newFile->set_Public( true );
    else
        newFile->set_Public( false );

    tokens = tokens[0].split( "/" );

    newFile->setName( tokens[1] );

    if( tokens[0] == "Music" )
        newFile->setType( FileType::MUSIC );
    else if( tokens[0] == "Videos" )
        newFile->setType( FileType::VIDEO );
    else
        newFile->setType( FileType::IMAGE );


    if( newFile->getSize() + currentUser->getMemoryUsed() < currentUser->getTotalMemorySpace() )
    {
        currentUser->setMemoryUsed( currentUser->getMemoryUsed() + newFile->getSize() );
        currentUser->addFile( newFile );
    }
}

void UserBuilder::setTotalMemory( QString totalMemory )
{
    currentUser->setTotalMemorySpace( totalMemory.toLongLong() );
}

void UserBuilder::putUser()
{
    result.insert( currentUser->getUserName(), currentUser );
}

User* UserBuilder::getCurrentUser() const
{
    return currentUser;
}

QString UserBuilder::getEntryPoint() const
{
    return entryPoint;
}

QMap<QString, User*> UserBuilder::getResult() const
{
    return this->result;
}


UserBuilder::~UserBuilder()
{

}

