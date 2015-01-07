#ifndef LOADFILESERVICE_H
#define LOADFILESERVICE_H

#include <QThread>
#include <QList>
#include <QTcpServer>

#include "loadfileconnection.h"

namespace services
{
    class LoadFileService : public QThread
    {
        Q_OBJECT

    private:
        QTcpServer* serverSocket;
        QList<LoadFileConnection*>* connections;
        void run();

    public:
        LoadFileService();
        ~LoadFileService();
    };
}

#endif // LOADFILESERVICE_H
