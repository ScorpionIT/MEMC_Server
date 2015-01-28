#ifndef DLNAPROCESS_H
#define DLNAPROCESS_H

#include <QDir>
#include <QThread>
#include <QVector>
#include <QString>
#include <QProcess>
#include <QTcpSocket>
#include <QStringList>

#include "usermanager.h"
#include "genericprocess.h"

using namespace users;


namespace services
{
    namespace dlna
    {
        class DLNAProcess : public GenericProcess
        {
        private:
            User* user;

            bool setupDlna();
            void newDlnaShare();
            void getCurrentDlnaShare();
            void stopDlnaShare();

            virtual void run();
            virtual void closeConnection();

        public:
            DLNAProcess( QTcpSocket* client );
            virtual ~DLNAProcess();
        };
    }
}

#endif // DLNAPROCESS_H
