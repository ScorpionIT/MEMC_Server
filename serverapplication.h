#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <QCoreApplication>
#include <QString>
#include <QFile>
#include <QDir>

#include "./UserManagerBuilding/userfiledirector.h"
#include "./UserManagerBuilding/userbuilder.h"
#include "./connection/connectionmanager.h"
#include "./connection/connectionmanager.h"
#include "./services/loadfileservice.h"
#include "./services/fileservice.h"
#include "./services/loadfileservice.h"
#include "./services/userfilemanager.h"
#include "./services/dlnaservice.h"
#include "./services/StreamingService.h"
#include "usermanager.h"

using namespace connection;
using namespace services;
using namespace dlna;

class ServerApplication
{
private:
    qint64 memory;
    qint64 freeMemory;
    ConnectionManager* connectionManager;
    FileService* fileService;
    LoadFileService* loadFileService;
    UserFileManager* userFileManager;
    DLNAService* dlnaService;
    AdminService* adminService;
    StreamingService* streamingService;


public:
    ServerApplication( QString userFile, QString configFile );
    void start();
    ~ServerApplication();
};

#endif // SERVERAPPLICATION_H
