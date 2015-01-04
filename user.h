#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>

#include "mediafile.h"

class User
{
private:
    QString userName;
    unsigned long totalMemorySpace;
    unsigned long memoryUsed;
    QString userDirectory;
    QString passwd;
    bool onLine;
    int sessionID = -1;

    QList<MediaFile*>* mediaFiles;

    void setSessionID( int sessionID );
    void setIsOnline( bool online );

public:
     User();
     User( QString userName, unsigned long totalMemorySpace, QString userDirectory, QString passwd );

     bool isOnLine() const;
     bool isPasswdCorrect( QString passwd );
     QString getUserDirectory() const;
    ~User();

     QString getUserName() const;
     void setUserName( const QString& value );

     void setPasswd( QString passwd );

     unsigned long getTotalMemorySpace() const;
     void setTotalMemorySpace( unsigned long value );

     unsigned long getMemoryUsed() const;
     void setMemoryUsed( unsigned long value );

     void addFile( MediaFile* file );

     bool getOnLine() const;


     friend class Connection;
};

#endif // USER_H
