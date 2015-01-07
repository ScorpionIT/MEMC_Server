#ifndef LOADFILECONNECTION_H
#define LOADFILECONNECTION_H

#include <QTcpSocket>
#include <QThread>
#include <QString>

namespace services
{
    class LoadFileConnection : public QThread
    {
        Q_OBJECT

    private:
        QTcpSocket* client;
        void closeConnection();
        void run();

    public:
        LoadFileConnection( QTcpSocket* client );
        ~LoadFileConnection();
    };
}
#endif // LOADFILECONNECTION_H
