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
    ConnectionManager* connectionManager;
    FileService* fileService;
    LoadFileService* loadFileService;
    UserFileManager* userFileManager;


public:
    ServerApplication( QString userFile );
    void start();
    ~ServerApplication();
};

#endif // SERVERAPPLICATION_H
