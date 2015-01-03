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
    void closed ();

public:
    Connection(QTcpSocket* client );

    void run();
};

#endif // CONNECTION_H
