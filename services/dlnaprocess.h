#ifndef DLNAPROCESS_H
#define DLNAPROCESS_H

#include <QDir>
#include <QThread>
#include <QVector>
#include <QString>
#include <QProcess>
#include <QTcpSocket>
#include <QStringList>
#include <QStringList>

#include "usermanager.h"

using namespace users;

namespace services
{
    namespace dlna
    {
        class DLNAProcess : public QThread
        {
        private:
            QTcpSocket* client;
            User* user;

            void run();
            bool setupDlna();
            void newDlnaShare();
            void getCurrentDlnaShare();
            void stopDlnaShare();

        public:
            DLNAProcess( QTcpSocket* client );
            ~DLNAProcess();
        };
    }
}

#endif // DLNAPROCESS_H
