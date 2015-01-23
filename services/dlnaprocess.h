#ifndef DLNAPROCESS_H
#define DLNAPROCESS_H

#include <QThread>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QTcpSocket>
#include "genericprocess.h"

namespace services
{
    namespace dlna
    {
        class DLNAProcess : public GenericProcess
        {
        private:
            virtual void run();
            virtual void closeConnection();

        public:
            DLNAProcess( QTcpSocket* client );
            virtual ~DLNAProcess();
        };
    }
}

#endif // DLNAPROCESS_H
