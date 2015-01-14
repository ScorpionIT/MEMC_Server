#ifndef USERFILEMANAGER_H
#define USERFILEMANAGER_H

#include <QThread>
#include <QTcpServer>
#include "userfilemanagerconnection.h"

class UserFileManager : public QThread
{
    Q_OBJECT

private:
    QTcpServer* serverSocket;
    QList<UserFileManagerConnection*>* connections;
    void run();

public:
    UserFileManager();
    ~UserFileManager();
};

#endif // USERFILEMANAGER_H
