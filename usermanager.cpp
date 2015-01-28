#include "usermanager.h"
#include <QDebug>
#include <QMutexLocker>
#include <QProcess>

using namespace users;

UserManager* UserManager::_userManager = nullptr;
QMap<QString, User*>* UserManager::users = new QMap<QString, User*>();
QReadWriteLock* UserManager::lock = new QReadWriteLock();
QMap<QString, MediaFile*>* UserManager::publicMediaFiles_m = new QMap<QString, MediaFile*>;
QMap<QString, MediaFile*>* UserManager::publicMediaFiles_v = new QMap<QString, MediaFile*>;
QMap<QString, MediaFile*>* UserManager::publicMediaFiles_i = new QMap<QString, MediaFile*>;

UserManager::UserManager()
{

}

void UserManager::addUser( User* user )
{
    QWriteLocker writeLocker( lock );
    users->insert( user->getUserName(), user );
}

void UserManager::removeUser( QString user )
{
    QWriteLocker writeLocker( lock );

    QString path = users->value( user )->getUserDirectory();


    QProcess rm;

    QStringList parameters;
    parameters.push_back( "-rf");
    parameters.push_back( path );

    rm.start( "rm", parameters );
    rm.waitForFinished( -1 );

    delete users->value( user );

    users->remove( user );
}

UserManager* UserManager::getInstance()
{
    QReadLocker locker( lock );
    if( _userManager == nullptr )
    {
        _userManager = new UserManager();

    }
    return _userManager;
}

void UserManager::initiate( QMap<QString, User*> fields )
{
    for( QMap<QString, User*>::iterator it = fields.begin(); it != fields.end(); it++ )
    {
        users->insert( it.key(), it.value() );

        publicMediaFiles_m->unite( ( *(it.value()->getPublicFiles( FileType::MUSIC ) ) ) );
        publicMediaFiles_v->unite( ( *(it.value()->getPublicFiles( FileType::VIDEO ) ) ) );
        publicMediaFiles_i->unite( ( *(it.value()->getPublicFiles( FileType::IMAGE ) ) ) );
    }
}

void UserManager::takeLock()
{
    lock->lockForRead();
}

void UserManager::leaveLock()
{
    lock->unlock();
}

User* UserManager::getUser(QString user)
{
    QReadLocker locker( lock );

    return (*users)[user];
}

bool UserManager::isAUser( QString user ) const
{
    return (*users).contains( user );
}

QList<MediaFile*>* UserManager::getPublicMediaFiles(QString userToKeepOut , FileType type) const
{
    QList<MediaFile*>* mediaList = new QList<MediaFile*>();

    for( QMap<QString, User*>::iterator it = users->begin(); it != users->end(); it++ )
    {
        if( it.key() != userToKeepOut )
        {
            QMap<QString, MediaFile*>* files;
            QMap<QString, MediaFile*>::iterator it2;

            switch( type )
            {
            case MUSIC:
                files = it.value()->getPublicFiles( FileType::MUSIC );
                it2 = files->begin();
                break;
            case VIDEO:
                files = it.value()->getPublicFiles( FileType::VIDEO );
                it2 = files->begin();
                break;
            case IMAGE:
                files = it.value()->getPublicFiles( FileType::IMAGE );
                it2 = files->begin();
                break;
            default:
                break;
            }
            if( !files->empty() )
                for( ; it2 != files->end(); it2++ )
                    mediaList->push_back( ( *it2 ) );
        }
    }

    return mediaList;
}

int UserManager::getNumOfUsers() const
{
    return this->users->size();
}

UserManager::~UserManager()
{
    if( users != nullptr )
        delete users;
    if( lock != nullptr )
        delete lock;
    if( _userManager != nullptr )
        delete _userManager;
    if( publicMediaFiles_m != nullptr )
        delete publicMediaFiles_m;
    if( publicMediaFiles_v != nullptr )
        delete publicMediaFiles_v;
    if( publicMediaFiles_i != nullptr )
        delete publicMediaFiles_i;
}

