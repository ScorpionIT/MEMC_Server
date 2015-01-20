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
        void run();
        void handleDeletingUsers();
        void handleEditingMemory();

    public:
        AdminService();
        ~AdminService();
    };
}
#endif // ADMINSERVICE_H
