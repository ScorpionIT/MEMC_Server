#include "serverapplication.h"

#include <QDebug>
using namespace UserManagerBuilding;


ServerApplication::ServerApplication( QString userFile , QString configFile )
{
    QFile config( configFile );
    if ( !config.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "unable to read config file";
        qApp->quit();
    }
    QTextStream in( &config );
    memory = QString( in.readLine() ).toLong();
    freeMemory = QString( in.readLine() ).toLong();
    config.close();

    UserBuilder* builder = new UserBuilder();
    UserFileDirector* director = new UserFileDirector( builder, userFile );
    director->startBuilding();

    UserManager::initiate( builder->getResult() );

    connectionManager = new ConnectionManager();
    fileService = new FileService();
    loadFileService = new LoadFileService();
    userFileManager = new UserFileManager();
    this->streamingService = new StreamingService();

    QFile usFile( userFile );
    if( !usFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "unable to read user file";
        qApp->quit();
    }
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
    this->streamingService->start();
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

