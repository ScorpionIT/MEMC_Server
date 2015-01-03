#include "usermanager.h"

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

void UserManager::initiate( QString* fields )
{
    users->insert( fields[0], new User( fields[0], 1000, QString("/home"), fields[2] ) );
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

