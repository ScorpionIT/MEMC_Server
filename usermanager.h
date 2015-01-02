#include "user.h"
#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QMap>
#include <QString>

class UserManager
{
private:
    static QMap<QString, User*>* users;
    static UserManager* _userManager;

    UserManager();

public:
    static UserManager* getInstance();
    static void initiate(QString* fields );
    User* getUser( QString user );
    bool isAUser( QString user ) const;
    ~UserManager();
};

#endif // USERMANAGER_H


