#ifndef GENERICPROCESS_H
#define GENERICPROCESS_H

#include <QThread>
#include <QTcpSocket>

namespace services
{
    class GenericProcess : public QThread
    {
        Q_OBJECT

    protected:
        QTcpSocket* client;
        virtual void closeConnection() = 0;
        virtual void run() = 0;

    public:
        GenericProcess( QTcpSocket* client ) : QThread ()
        {
            client->setParent( nullptr );
            this->client = client;
        }

        virtual ~GenericProcess()
        {
            delete client;
        }
    };
}

#endif // GENERICPROCESS_H



