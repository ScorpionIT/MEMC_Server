#ifndef DLNAPROCESS_H
#define DLNAPROCESS_H

#include <QThread>
#include <QVector>
#include <QString>
#include <QStringList>
#include "usermanager.h"

namespace services
{
    class DLNAProcess : public QThread
    {
    private:
        QStringList files;
        QString user;

    protected:
        void run();

    public:
        DLNAProcess( QStringList files, QString user );
        ~DLNAProcess();
    };
}

#endif // DLNAPROCESS_H
