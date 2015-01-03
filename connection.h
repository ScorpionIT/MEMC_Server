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
    QString user;
    static const unsigned long SESSION_TIMER;
    static int computeSessionID();
    void closeConnection();

signals:
    void closed ();

public:
    Connection(QTcpSocket* client );

    void run();
};

#endif // CONNECTION_H
