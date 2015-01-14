#include "usermanager.h"
#include <QDebug>
#include <QMutexLocker>

using namespace user;

UserManager* UserManager::_userManager = nullptr;
QMap<QString, User*>* UserManager::users = new QMap<QString, User*>();
QReadWriteLock* UserManager::lock = new QReadWriteLock();

UserManager::UserManager()
{

}

void UserManager::addUser( User* user )
{
    QWriteLocker writeLocker( lock );
    users->insert( user->getUserName(), user );
}

void UserManager::removeUser(QString user)
{
    QWriteLocker writeLocker( lock );
    users->remove( user );
}

UserManager* UserManager::getInstance()
{
    QReadLocker locker( lock );
    if( _userManager == nullptr )
    {
        _userManager = new UserManager();

    }
    return _userManager;
}

void UserManager::initiate(QMap<QString, User*> fields )
{
    for( QMap<QString, User*>::iterator it = fields.begin(); it != fields.end(); it++ )
    {
        users->insert( it.key(), it.value() );
    }
}

void UserManager::takeLock()
{
    lock->lockForRead();
}

void UserManager::leaveLock()
{
    lock->unlock();
}

User* UserManager::getUser(QString user)
{
    QReadLocker locker( lock );

    return (*users)[user];
}

bool UserManager::isAUser( QString user ) const
{
    return (*users).contains( user );
}

int UserManager::getNumOfUsers() const
{
    return this->users->size();
}

UserManager::~UserManager()
{
    if( users != nullptr )
        delete users;

    if( lock != nullptr )
        delete lock;

    if( _userManager != nullptr )
        delete _userManager;
}

