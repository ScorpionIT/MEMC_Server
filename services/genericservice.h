#ifndef GENERICSERVICE_H
#define GENERICSERVICE_H

#include <QThread>
#include <QTcpServer>
#include "genericprocess.h"

namespace services
{
class GenericService : public QThread
{
    Q_OBJECT

protected:
    QTcpServer* serverSocket;
    QList<GenericProcess*>* connections;
    virtual void run() = 0;

public:
    GenericService() : QThread ()
    {
        serverSocket = new QTcpServer();
        connections = new QList<GenericProcess*>();
        this->serverSocket->moveToThread( this );
    }

    virtual ~GenericService()
    {
        for( QList<GenericProcess*>::iterator it = connections->begin(); it != connections->end(); it++ )
            delete (*it);

        delete connections;
        delete serverSocket;
    }

};

}
#endif // GENERICSERVICE_H

