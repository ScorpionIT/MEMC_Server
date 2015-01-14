#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"

#include <QString>
#include <QReadWriteLock>
#include <QReadLocker>

namespace users
{
    class UserManager
    {
    private:
        static QMap<QString, User*>* users;
        static UserManager* _userManager;

        static QReadWriteLock* lock;

        UserManager();

        void addUser( User* users );
        void removeUser( QString users );

    public:
        static UserManager* getInstance();
        static void initiate( QMap<QString, User*> fields );

        static void takeLock();
        static void leaveLock();

        User* getUser( QString users );
        bool isAUser( QString users ) const;
        int getNumOfUsers() const;
        ~UserManager();
    };
}
#endif // USERMANAGER_H


