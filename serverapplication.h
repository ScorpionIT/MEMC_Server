#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <QString>
#include "connectionmanager.h"

class ServerApplication
{
private:
    ConnectionManager* connectionManager;

public:
    ServerApplication( QString userFile );
    void start();
    ~ServerApplication();
};

#endif // SERVERAPPLICATION_H
