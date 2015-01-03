#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
#include <QThread>
#include <QString>

class Connection : public QThread
{
Q_OBJECT

private:
    QTcpSocket* client;
    static const unsigned long SESSION_TIMER;

signals:
    void closed ( Connection* conn );

public:
    Connection( QTcpSocket* client );

    void run() Q_DECL_OVERRIDE;
};

#endif // CONNECTION_H
