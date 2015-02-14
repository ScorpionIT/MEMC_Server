#ifndef USERFILEMANAGERCONNECTION_H
#define USERFILEMANAGERCONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QStringList>
#include <QFile>
#include <QProcess>

#include "usermanager.h"
#include "genericprocess.h"

using namespace users;

namespace services
{
    class UserFileManagerProcess: public GenericProcess
    {
    private:
        User* user;
        QStringList filesToChangeScope;
        QMap<QString, MediaFile*>* chageScopeMediaFileList;
        bool toPublic;

        virtual void run();
        virtual void closeConnection();
        void handleDeletingFiles();
        void handleChangingScope();
        void restoreScope(int n = -1);

    public:
        UserFileManagerProcess( QTcpSocket* client );
        virtual ~UserFileManagerProcess();
    };
}
#endif // USERFILEMANAGERCONNECTION_H
