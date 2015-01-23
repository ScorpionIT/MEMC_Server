#ifndef LOADFILECONNECTION_H
#define LOADFILECONNECTION_H

#include <QTcpSocket>
#include <QThread>
#include <QString>
#include "genericprocess.h"

namespace services
{
    class LoadFileProcess : public GenericProcess
    {
        Q_OBJECT

    private:
        virtual void closeConnection();
        virtual void run();

    public:
        LoadFileProcess( QTcpSocket* client );
        ~LoadFileProcess();
    };
}
#endif // LOADFILECONNECTION_H
