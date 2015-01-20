#include "adminservice.h"
using namespace services;

#include <QFile>
#include "../usermanager.h"

AdminService::AdminService()
{

}

AdminService::~AdminService()
{

}



void AdminService::run()
{
    this->serverSocket = new QTcpServer();
    bool error = this->serverSocket->listen( QHostAddress::Any, 80000 );
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

        QFile adminCredentials( "./admin.txt" );
        QTextStream in( &adminCredentials );

        QString line = in.readLine();

        this->admin->waitForReadyRead();
        QStringList credentials = this->admin->readLine().chop( 1 ).split( "$" );

        if( credentials.size() != 2 )
        {
            this->admin->write( "you are not the admin, bye\n" );
            this->admin->waitForBytesWritten();
            this->admin->close();
        }
        else if( credentials[0] != line )
        {
            this->admin->write( "you are not the admin, bye\n" );
            this->admin->waitForBytesWritten();
            this->admin->close();
        }
        else if( ( line = in.readLine() ) != credentials[1] )
        {
            this->admin->write( "you are not the admin, bye\n" );
            this->admin->waitForBytesWritten();
            this->admin->close();
        }
        else
        {
            this->admin->write( "hi, admin\n" );
            this->admin->waitForBytesWritten();
            this->admin->write( "remove=1,edit memory=2, finish=3\n" );
            this->admin->waitForBytesWritten();

            int choice = QString( this->admin->readLine().chop( 1 ) ).toInt();

            switch( choice )
            {
            case 1:


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

    int errorCounter = 0;

    while( users::UserManager::getInstance()->isAUser( user ) )
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
    }

    int memory = users::UserManager::getInstance()->getUser( user )->getTotalMemorySpace();
    users::UserManager::getInstance()->removeUser( user );


}




