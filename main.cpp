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
    QString path2 = dir.homePath();

    path += "/Dropbox/Progetto Condiviso/Incigneria/users.txt";
    path2 += "/Dropbox/Progetto Condiviso/Incigneria/config.txt";

    ServerApplication* server = new ServerApplication( path, path2 );
    server->start();

    return app.exec();
}

