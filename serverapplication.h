#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <QString>
#include "./connection/connectionmanager.h"
#include "./services/fileservice.h"
#include "./services/loadfileservice.h"

using namespace connection;
using namespace services;


class ServerApplication
{
private:
    ConnectionManager* connectionManager;
    FileService* fileService;
    LoadFileService* loadFileService;


public:
    ServerApplication( QString userFile );
    void start();
    ~ServerApplication();
};

#endif // SERVERAPPLICATION_H
