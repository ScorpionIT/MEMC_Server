#ifndef USERBUILDER_H
#define USERBUILDER_H

#include "../user.h"
#include <QMap>

class UserBuilder
{
private:
    QString entryPoint;
    User* currentUser;
    QMap<QString, User*> result;
    unsigned long memory = 0;

public:
    UserBuilder();

    void setEntryPoint( QString entryPoint );
    void createUser( QString username );
    void createPasswd( QString passwd );
    void addFile( QString file );
    void setTotalMemory( QString totalMemory );
    void putUser();

    QMap<QString, User*> getResult() const;
    ~UserBuilder();
};

#endif // USERFILEBUILDER_H
