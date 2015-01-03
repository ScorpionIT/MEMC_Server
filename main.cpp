#include "connectionmanager.h"
#include "usermanager.h"
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QFile file("/home/Suinos/Dropbox/Progetto Condiviso/Incigneria/Server/MEMC_Server/users.txt");
    bool elia = file.open( QIODevice::ReadOnly );

    QTextStream in( &file );

    QString fields[3];
    int cont = 0;
    QString line = in.readLine();
    while( !line.isNull() )
    {
        fields[cont++] = line;
        line = in.readLine();
    }

    UserManager::initiate( fields );






    ConnectionManager* connectionManager = new ConnectionManager();
    connectionManager->start();
    return 0;
}

