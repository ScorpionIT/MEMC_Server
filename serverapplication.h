#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <QString>
#include "connectionmanager.h"
#include "fileservice.h"

class ServerApplication
{
private:
    ConnectionManager* connectionManager;
    FileService* fileService;


public:
    ServerApplication( QString userFile );
    void start();
    ~ServerApplication();
};

#endif // SERVERAPPLICATION_H
