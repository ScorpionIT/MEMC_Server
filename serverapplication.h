#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <QString>
#include "./connection/connectionmanager.h"
#include "./services/fileservice.h"
#include "./services/loadfileservice.h"
#include "./services/userfilemanager.h"

using namespace connection;
using namespace services;


class ServerApplication
{
private:
    qint64 memory;
    qint64 freeMemory;
    ConnectionManager* connectionManager;
    FileService* fileService;
    LoadFileService* loadFileService;
    UserFileManager* userFileManager;

    AdminService* adminService;


public:
    ServerApplication( QString userFile, QString configFile );
    void start();
    ~ServerApplication();
};

#endif // SERVERAPPLICATION_H
