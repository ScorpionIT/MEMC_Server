#ifndef DLNASERVICE_H
#define DLNASERVICE_H

#include "userservice.h"
#include "dlnaprocess.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

class DLNAService : public UserService
{
private:
    QTcpServer* serverSocket;
    bool run = false;
    QVector<DLNAProcess*>* processes;

public:
    DLNAService();
    void start();
    ~DLNAService();
};

#endif // DLNASERVICE_H
