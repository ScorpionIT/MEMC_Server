#include "serverapplication.h"
#include "./UserManagerBuilding/userfiledirector.h"
#include "./UserManagerBuilding/userbuilder.h"
#include "usermanager.h"
#include "connectionmanager.h"

#include <QFile>
#include <QDebug>

ServerApplication::ServerApplication( QString userFile )
{
    UserBuilder* builder = new UserBuilder();
    UserFileDirector* director = new UserFileDirector( builder, userFile );
    director->startBuilding();

    UserManager::initiate( builder->getResult() );

    connectionManager = new ConnectionManager();
}

void ServerApplication::start()
{
    connectionManager->start();
}

ServerApplication::~ServerApplication()
{
    delete connectionManager;
}

