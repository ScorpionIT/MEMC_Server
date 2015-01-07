#include "./connection/connectionmanager.h"
#include "usermanager.h"
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QDir>
#include "serverapplication.h"


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QDir dir;
    QString path = dir.homePath();

    path += "/Dropbox/Progetto Condiviso/Incigneria/users.txt";

    ServerApplication* server = new ServerApplication( path );
    server->start();

    return app.exec();
}

