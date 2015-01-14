#ifndef DLNAPROCESS_H
#define DLNAPROCESS_H

#include <QThread>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QTcpSocket>

namespace services
{
    namespace dlna
    {
        class DLNAProcess : public QThread
        {
        private:
            QTcpSocket* client;
            void run();

        public:
            DLNAProcess( QTcpSocket* client );
            ~DLNAProcess();
        };
    }
}

#endif // DLNAPROCESS_H
