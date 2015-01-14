#ifndef USERFILEMANAGERCONNECTION_H
#define USERFILEMANAGERCONNECTION_H

#include <QThread>
#include <QTcpSocket>


#include "../usermanager.h"
using namespace users;

class UserFileManagerConnection: public QThread
{
private:
    QTcpSocket* client;
    void run();

    User* user;

    void handleDeletingFiles();
    void handleChangingScope();

public:
    UserFileManagerConnection( QTcpSocket* client );
    ~UserFileManagerConnection();
};

#endif // USERFILEMANAGERCONNECTION_H
