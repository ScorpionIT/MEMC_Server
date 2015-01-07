#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>

#include "mediafile.h"
#include "./connection/connection.h"

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

    QList<MediaFile*>* musicFiles;
    QList<MediaFile*>* videoFiles;
    QList<MediaFile*>* imageFiles;

    void connect( int sessionID );
    void disconnect();
    void setSessionID( int sessionID );
    void setIsOnline( bool online );

public:
     User();
     User( QString userName, unsigned long totalMemorySpace, QString userDirectory, QString passwd );
     ~User();

     bool isOnLine( int sessionID ) const;
     bool isPasswdCorrect( QString passwd );

     QString getUserDirectory() const;
     void setUserDirectory( QString path );

     QString getUserName() const;
     void setUserName( const QString& value );

     void setPasswd( QString passwd );

     unsigned long getTotalMemorySpace() const;
     void setTotalMemorySpace( unsigned long value );

     unsigned long getMemoryUsed() const;
     void setMemoryUsed( unsigned long value );

     void addFile( MediaFile* file );

     bool getOnLine() const;

     QList<MediaFile*>* getMediaFiles( FileType type ) const;

     friend class connection::Connection;

};

#endif // USER_H
