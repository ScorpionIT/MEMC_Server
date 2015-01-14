#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>
#include <QMap>
#include "mediafile.h"

#include "connection/connection.h"
#include "services/dlnaprocess.h"

namespace user
{


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

        QMap<QString, MediaFile*>* musicFiles;
        QMap<QString, MediaFile*>* videoFiles;
        QMap<QString, MediaFile*>* imageFiles;

        QMap<QString, services::dlna::DLNAProcess*>* dlnaSharing; //= new QMap<QString, DLNAProcess*>();

        void connect( int sessionID );
        void disconnect();
        void setSessionID( int sessionID );
        void setIsOnline( bool online );

    public:
         User();
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

         QMap<QString, MediaFile*>* getMediaFiles( FileType type ) const;

         void addDLNASharing( QString pid, services::dlna::DLNAProcess* sharing );
         bool removeDLNASharing( QString pid );

         friend class connection::Connection;

    };
}
#endif // USER_H
