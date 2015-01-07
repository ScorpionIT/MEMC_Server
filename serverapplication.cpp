#include "serverapplication.h"
#include "./UserManagerBuilding/userfiledirector.h"
#include "./UserManagerBuilding/userbuilder.h"
#include "usermanager.h"
#include "./connection/connectionmanager.h"
#include "./services/loadfileservice.h"

#include <QFile>
#include <QDebug>
using namespace UserManagerBuilding;


ServerApplication::ServerApplication( QString userFile )
{
    UserBuilder* builder = new UserBuilder();
    UserFileDirector* director = new UserFileDirector( builder, userFile );
    director->startBuilding();

    UserManager::initiate( builder->getResult() );

    connectionManager = new ConnectionManager();
    fileService = new FileService();
    loadFileService = new LoadFileService();
}

void ServerApplication::start()
{
    connectionManager->start();
    fileService->start();
    loadFileService->start();
}

ServerApplication::~ServerApplication()
{
    delete connectionManager;
    delete fileService;
    delete loadFileService;
}
