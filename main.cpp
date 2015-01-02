#include "connectionmanager.h"
#include <QFile>

int main(int argc, char *argv[])
{
    QFile file("/home/Suinos/Dropbox/Progetto Condiviso/Incigneria/Server/MEMC_Server/users.txt");

    QTextStream in( &file );

    QString fields[3];
    int cont = 0;
    while( !in.atEnd() )
    {
        fields[cont++] = in.readLine();
    }






    ConnectionManager* connectionManager = new ConnectionManager();
    connectionManager->start();
    return 0;
}

