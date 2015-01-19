#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

namespace services
{
    class FileService : public QThread
    {

    private:
        int counter;
        QTcpServer* serverSocket;
        QTcpSocket* client;
        void run();

    public:
        FileService();
        ~FileService();
    };

}
#endif // FILESERVICE_H
