#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include "./services/adminservice.h"

#include <QString>
#include <QReadWriteLock>
#include <QReadLocker>

namespace users
{
    class UserManager
    {
    private:
        static QMap<QString, User*>* users;
        static QMap<QString, MediaFile*>* publicMediaFiles_m;
        static QMap<QString, MediaFile*>* publicMediaFiles_v;
        static QMap<QString, MediaFile*>* publicMediaFiles_i;
        static UserManager* _userManager;

        static QReadWriteLock* lock;

        UserManager();

    public:
        static UserManager* getInstance();
        static void initiate( QMap<QString, User*> fields );

        static void takeLock();
        static void leaveLock();

        void addUser( User* users );
        void removeUser( QString users );

        User* getUser( QString users );
        bool isAUser( QString users ) const;
        QList<MediaFile*>* getPublicMediaFiles( QString userToKeepOut, FileType type ) const;
        int getNumOfUsers() const;
        ~UserManager();
    };
}
#endif // USERMANAGER_H


