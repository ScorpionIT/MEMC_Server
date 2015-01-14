#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"

#include <QString>
#include <QReadWriteLock>
#include <QReadLocker>

namespace user
{
    class UserManager
    {
    private:
        static QMap<QString, User*>* users;
        static UserManager* _userManager;

        static QReadWriteLock* lock;

        UserManager();

        void addUser( User* user );
        void removeUser( QString user );

    public:
        static UserManager* getInstance();
        static void initiate( QMap<QString, User*> fields );

        static void takeLock();
        static void leaveLock();

        User* getUser( QString user );
        bool isAUser( QString user ) const;
        int getNumOfUsers() const;
        ~UserManager();
    };
}
#endif // USERMANAGER_H


