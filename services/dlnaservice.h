#ifndef DLNASERVICE_H
#define DLNASERVICE_H

#include "dlnaprocess.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QThread>

namespace services
{
    namespace dlna
    {
        class DLNAService : public QThread
        {
        private:
            QTcpServer* serverSocket;
            QVector<DLNAProcess*>* connections;
            void run();

        public:
            DLNAService();
            ~DLNAService();
        };
    }
}
#endif // DLNASERVICE_H
