#ifndef USERFILEMANAGERCONNECTION_H
#define USERFILEMANAGERCONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QStringList>
#include <QFile>
#include <QProcess>

#include "../usermanager.h"

using namespace users;

class UserFileManagerConnection: public QThread
{
private:
    QTcpSocket* client;
    User* user;
    QStringList filesToChangeScope;
    QMap<QString, MediaFile*>* chageScopeMediaFileList;
    bool toPublic;

    void run();
    void handleDeletingFiles();
    void handleChangingScope();
    void restoreScope(int n = -1);

public:
    UserFileManagerConnection( QTcpSocket* client );
    ~UserFileManagerConnection();
};

#endif // USERFILEMANAGERCONNECTION_H
