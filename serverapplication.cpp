#include "serverapplication.h"
#include "./UserManagerBuilding/userfiledirector.h"
#include "./UserManagerBuilding/userbuilder.h"
#include "usermanager.h"
#include "./connection/connectionmanager.h"
#include "./services/loadfileservice.h"

#include <QFile>
#include <QDebug>
#include <QDir>
using namespace UserManagerBuilding;


ServerApplication::ServerApplication( QString userFile , QString configFile )
{
    QFile config( configFile );
    QTextStream in( &config );

    memory = QString( in.readLine() ).toLong();
    freeMemory = QString( in.readLine() ).toLong();

    UserBuilder* builder = new UserBuilder();
    UserFileDirector* director = new UserFileDirector( builder, userFile );
    director->startBuilding();

    UserManager::initiate( builder->getResult() );

    connectionManager = new ConnectionManager();
    fileService = new FileService();
    loadFileService = new LoadFileService();
    userFileManager = new UserFileManager();
    QFile usFile( userFile );
    while( !usFile.open( QIODevice::ReadOnly ) );
    QString entryPoint = QTextStream( &usFile ).readLine();
    usFile.close();
    adminService = new AdminService( entryPoint, userFile, configFile );
    this->dlnaService = new DLNAService();
}

void ServerApplication::start()
{
    connectionManager->start();
    fileService->start();
    loadFileService->start();
    userFileManager->start();
    adminService->start();
    this->dlnaService->start();
}

ServerApplication::~ServerApplication()
{
    delete connectionManager;
    delete fileService;
    delete loadFileService;
    delete userFileManager;
    delete adminService;
    delete this->dlnaService;
}

