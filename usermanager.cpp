#include "usermanager.h"

UserManager* UserManager::_userManager = nullptr;

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
    users.insert( fields[0], new User( fields[0], fields[1], "/home", fields[2] ) );
}

User UserManager::getUser(QString user)
{
    return this->users[user];
}

bool UserManager::isAUser( QString user ) const
{
    return this->users.contains( user );
}

UserManager::~UserManager()
{

}

