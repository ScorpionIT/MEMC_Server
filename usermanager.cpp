#include "usermanager.h"
#include <QDebug>

UserManager* UserManager::_userManager = nullptr;
QMap<QString, User*>* UserManager::users = new QMap<QString, User*>();

UserManager::UserManager()
{

}

UserManager* UserManager::getInstance()
{
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
//        qDebug() << it.key();
//        qDebug() << it.value()->getUserName();
//        qDebug() << it.value()->getTotalMemorySpace();

        users->insert( it.key(), it.value() );
    }
}

User* UserManager::getUser(QString user)
{
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

}

