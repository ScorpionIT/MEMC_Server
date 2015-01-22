#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

namespace services
{
    class AdminService : public QThread
    {
    private:
        QTcpServer* serverSocket;
        QTcpSocket* admin;
        QString entryPoint;
        QString userFile;
        QString configFile;
        void run();
        void handleDeletingUsers();
        void handleAddingUser();

    public:
        AdminService( QString entryPoint, QString userFile, QString configFile );
        ~AdminService();
    };
}
#endif // ADMINSERVICE_H
