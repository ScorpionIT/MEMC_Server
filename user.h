#ifndef USER_H
#define USER_H

#include <QString>

class User
{
private:
    QString id;
    unsigned long memorySpace;
    QString userDirectory;
    QString passwd;
    bool onLine;

public:
    User();
     User( QString id, unsigned long memorySpace, QString userDirectory, QString passwd );

     bool isOnLine() const;
     bool isPasswdCorrect( QString passwd );
     QString getUserDirectory() const;
    ~User();
};

#endif // USER_H
