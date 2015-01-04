#include "connectionmanager.h"
#include "usermanager.h"
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include "serverapplication.h"


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    //    QFile file("/home/Suinos/Scrivania/CDL/Software Engineering/Progetto/MEMC_Server/users.txt");
    //    bool elia = file.open( QIODevice::ReadOnly );

    //    QTextStream in( &file );

    //    QString fields[3];
    //    int cont = 0;
    //    QString line = in.readLine();
    //    while( cont < 3 )
    //    {
    //        fields[cont++] = line;
    //        line = in.readLine();
    //    }

    //UserManager::initiate( fields );

    ServerApplication* server = new ServerApplication( "/home/Suinos/Scrivania/CDL/Software Engineering/Progetto/MEMC_Server/users.txt" );
    //server

    server->start();





    //ConnectionManager* connectionManager = new ConnectionManager();
    //connectionManager->start();
    return app.exec();
}

