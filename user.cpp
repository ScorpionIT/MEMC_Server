#include "user.h"

User::User()
{

}

User::User(QString id, unsigned long memorySpace, QString userDirectory, QString passwd)
{
    this->id = id;
    this->memorySpace = memorySpace;
    this->userDirectory = userDirectory;
    this->passwd = passwd;

}

bool User::isOnLine() const
{
    return this->onLine;
}

bool User::isPasswdCorrect(QString passwd)
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

