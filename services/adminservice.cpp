#include "adminservice.h"
using namespace services;

#include <QFile>
#include <QProcess>
#include <QDir>
#include "../usermanager.h"

AdminService::AdminService( QString entryPoint , QString userFile, QString configFile )
{
    this->entryPoint = entryPoint;
    this->userFile = userFile;
    this->configFile = configFile;
}

AdminService::~AdminService()
{

}

void AdminService::run()
{
    this->serverSocket = new QTcpServer();
    bool error = this->serverSocket->listen( QHostAddress::Any, 88888 );
    if (!error)
    {
        qDebug() << "errore listen";
    }
    while( true )
    {
        if ( !this->serverSocket->hasPendingConnections() )
            this->serverSocket->waitForNewConnection( -1, 0 );

        this->admin = this->serverSocket->nextPendingConnection();

        this->admin->write( QString( "username and passwd, please[split with $]\n" ).toUtf8() );
        this->admin->waitForBytesWritten();

        qDebug() << QDir::currentPath();
        QFile adminCredentials( QDir::currentPath() + "/admin.txt" );

        while( !adminCredentials.open( QIODevice::ReadOnly ) );

        QTextStream in( &adminCredentials );

        QString line = in.readLine();

        this->admin->waitForReadyRead();
        QString adminNameAndPass = this->admin->readLine();
        adminNameAndPass.chop( 2 );
        QStringList credentials = ( adminNameAndPass ).split( "$" );

        if( credentials.size() != 2 )
        {
            qDebug() << "dimensione";
            this->admin->write( "you are not the admin, bye\n" );
            this->admin->waitForBytesWritten();
            this->admin->close();
        }
        else if( credentials[0] != line )
        {
            qDebug() << "username \n" << "vero " << line << " inserito " << credentials[0];
            this->admin->write( "you are not the admin, bye\n" );
            this->admin->waitForBytesWritten();
            this->admin->close();
        }
        else if( ( line = in.readLine() ) != credentials[1] )
        {
            qDebug() << "passwd";
            this->admin->write( "you are not the admin, bye\n" );
            this->admin->waitForBytesWritten();
            this->admin->close();
        }
        else
        {
            this->admin->write( "hi, admin\n" );
            this->admin->waitForBytesWritten();

            bool finished = false;

            while( !finished )
            {
                this->admin->write( "remove=1,edit memory=2, add user=3, finish=4\n" );
                this->admin->waitForBytesWritten();

                this->admin->waitForReadyRead();
                QString inputChoice = this->admin->readLine();
                inputChoice.chop( 2 );

                int choice = inputChoice.toInt();
                qDebug() << "choice " << choice;

                switch( choice )
                {
                case 1:
                    this->handleDeletingUsers();
                    break;
                case 2:
                    break;
                case 4:
                    this->admin->close();
                    finished = true;
                default:
                    break;

                }
            }
        }
    }

}

void AdminService::handleDeletingUsers()
{
    this->admin->write( "who?\n" );
    this->admin->waitForBytesWritten();

    this->admin->waitForReadyRead();
    QString user = this->admin->readLine();
    user.chop( 2 );

    int errorCounter = 0;

    while( !users::UserManager::getInstance()->isAUser( user ) )
    {
        errorCounter++;
        if( errorCounter > 4 )
        {
            this->admin->write( "error, bye\n" );
            this->admin->waitForBytesWritten();
            this->admin->close();
            return;
        }
        this->admin->write( "error, try again\n" );
        this->admin->waitForBytesWritten();
        this->admin->waitForReadyRead();
        user = this->admin->readLine();
        user.chop( 2 );
    }

    int freedMemory = users::UserManager::getInstance()->getUser( user )->getTotalMemorySpace();
    users::UserManager::getInstance()->removeUser( user );

    QFile* file = new QFile( QString( userFile ) );

    while( !file->open( QIODevice::ReadOnly ) );

    QStringList userTokens;

    QTextStream* in = new QTextStream( file );

    QString line = in->readLine();
    userTokens.push_back( line );

    bool isTheUser = false;
    while( !in->atEnd() )
    {
        line = in->readLine();
        if( line == QString( "user=" + user ) )
        {
            userTokens.pop_back();
            isTheUser = true;
        }
        else if( !isTheUser )
        {
            userTokens.push_back( line );
        }
        else if( isTheUser && line == "END" )
            isTheUser = false;

    }

    file->close();
    delete file;
    delete in;

    QProcess rm;

    QStringList arguments;
    arguments.push_back( entryPoint + "users.txt" );

    rm.start( "rm", arguments );
    rm.waitForFinished( -1 );

    QProcess touch;
    touch.start( "touch", arguments );
    touch.waitForFinished( -1 );

    file = new QFile( userFile );

    while( !file->open( QIODevice::WriteOnly ) );


    for( int i = 0; i < userTokens.size() - 1; i++ )
        file->write( QString( userTokens[i] + "\n" ).toUtf8() );

    file->write( userTokens.back().toUtf8() );

    file->close();
    delete file;

    this->admin->write( "done\n" );
    this->admin->waitForBytesWritten();
}




