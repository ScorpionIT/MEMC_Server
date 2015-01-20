#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>
#include <QMap>
#include "mediafile.h"

#include "connection/connection.h"
#include "services/dlnaprocess.h"

namespace users
{


    class User
    {
    private:
        QString userName;
        qint64 totalMemorySpace;
        qint64 memoryUsed;
        QString userDirectory;
        QString passwd;
        bool onLine;
        int sessionID = -1;

        QMap<QString, MediaFile*>* musicFiles;
        QMap<QString, MediaFile*>* videoFiles;
        QMap<QString, MediaFile*>* imageFiles;

        QMap<QString, MediaFile*>* publicMusicFiles;
        QMap<QString, MediaFile*>* publicVideoFiles;
        QMap<QString, MediaFile*>* publicImageFiles;

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

         qint64 getTotalMemorySpace() const;
         void setTotalMemorySpace( qint64 value );

         qint64 getMemoryUsed() const;
         void setMemoryUsed( qint64 value );

         void addFile( MediaFile* file );

         bool getOnLine() const;

         QMap<QString, MediaFile*>* getMediaFiles( FileType type ) const;
         QMap<QString, MediaFile*>* getPublicFiles( FileType type ) const;

         MediaFile* takeFile( QString name );

         void addDLNASharing( QString pid, services::dlna::DLNAProcess* sharing );
         bool removeDLNASharing( QString pid );

         friend class connection::Connection;

    };
}
#endif // USER_H
