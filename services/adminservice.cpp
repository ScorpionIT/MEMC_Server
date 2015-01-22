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
                this->admin->write( "remove=1, add user=2, finish=3\n" );
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
                    this->handleAddingUser();
                    break;
                case 3:
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

    file = new QFile( configFile );

    while( !file->open(QIODevice::ReadOnly ) );
    QStringList listOfConfigToken;

    listOfConfigToken << file->readLine();
    listOfConfigToken << file->readLine();
    file->close();
    delete file;

    QProcess configRm;
    configRm.start( "rm", QStringList() << configFile );
    configRm.waitForFinished( -1 );

    QProcess configTouch;
    configTouch.start( "touch", QStringList() << configFile );
    configTouch.waitForFinished( -1 );

    file = new QFile( configFile );

    while( !file->open( QIODevice::WriteOnly | QIODevice::Append ) );

    file->write( listOfConfigToken[0].toUtf8() );
    file->write( QString::number( listOfConfigToken[1].toInt() + freedMemory ).toUtf8() );
    file->close();

    delete file;

    this->admin->write( "done\n" );
    this->admin->waitForBytesWritten();
}

static QString makePassword()
{
    return QString( "ciao" );
}

void AdminService::handleAddingUser()
{

    admin->write( "nickname\n" );
    admin->waitForBytesWritten();

    admin->waitForReadyRead();
    QString user = admin->readLine();
    user.chop( 2 );

    QFile config( configFile );

    while( !config.open( QIODevice::ReadOnly ) );

    QTextStream in( &config );

    qint64 totMem = in.readLine().toLong();
    qint64 usedMem = in.readLine().toLong();

    qint64 freeMem = totMem - usedMem;

    config.close();
    admin->write( QString( "free memory=" + QString::number( freeMem ) + "\n" ).toUtf8() );
    admin->waitForBytesWritten();

    admin->write( "how much\n" );
    admin->waitForBytesWritten();

    admin->waitForReadyRead();
    QString answer = admin->readLine();
    answer.chop( 2 );


    while( answer.toLong() > freeMem )
    {
        admin->write( "how much\n" );
        admin->waitForBytesWritten();

        admin->waitForReadyRead();
        answer = admin->readLine();
        answer.chop( 2 );
    }

    QString passwd = makePassword();

    admin->write( QString( "passwd=" + passwd + "\n" ).toUtf8() );
    admin->waitForBytesWritten();

    users::User* newUser = new users::User;
    newUser->setUserName( user );
    newUser->setPasswd( passwd );
    newUser->setTotalMemorySpace( answer.toLong() );
    newUser->setUserDirectory( entryPoint + "/" + user );

    qDebug() << "sto creando in " << entryPoint + "/" + user;
    QProcess* mkdir = new QProcess();
    mkdir->start( "mkdir", QStringList() << entryPoint + "/" + user );
    mkdir->waitForFinished( -1 );
    delete mkdir;

    mkdir = new QProcess();
    mkdir->start( "mkdir", QStringList() << entryPoint + "/" + user + "/music" );
    mkdir->waitForFinished( -1 );
    delete mkdir;

    mkdir = new QProcess();
    mkdir->start( "mkdir", QStringList() << entryPoint +  "/" + user + "/videos" );
    mkdir->waitForFinished( -1 );
    delete mkdir;

    mkdir = new QProcess();
    mkdir->start( "mkdir", QStringList() << entryPoint + "/" + user + "/images" );
    mkdir->waitForFinished( -1 );
    delete mkdir;

    QProcess* touch = new QProcess();

    touch->start( "touch",  QStringList() << entryPoint + "/" + user + "/music/index.txt");
    touch->waitForFinished( -1 );
    delete touch;

    touch = new QProcess();
    touch->start( "touch",  QStringList() << entryPoint + "/" + user + "/videos/index.txt");
    touch->waitForFinished( -1 );
    delete touch;

    touch = new QProcess();
    touch->start( "touch",  QStringList() << entryPoint + "/" + user + "/images/index.txt");
    touch->waitForFinished( -1 );
    delete touch;

    QFile usFile( userFile );
    while( usFile.open( QIODevice::WriteOnly | QIODevice::Append ) );

    usFile.write( "\nBEGIN\n" );
    usFile.write( QString( "user=" + user + "\n" ).toUtf8() );
    usFile.write( QString( "passwd=" + passwd + "\n" ).toUtf8() );
    usFile.write( QString( "totmemory=" + answer + "\n" ).toUtf8() );
    usFile.write( "files=Music/index.txt\n" );
    usFile.write( "files=Videos/index.txt\n" );
    usFile.write( "files=Images/index.txt\n" );
    usFile.write( "END" );

    QFile* file = new QFile( configFile );

    while( !file->open(QIODevice::ReadOnly ) );
    QStringList listOfConfigToken;

    listOfConfigToken << file->readLine();
    listOfConfigToken << file->readLine();
    file->close();
    delete file;

    QProcess configRm;
    configRm.start( "rm", QStringList() << configFile );
    configRm.waitForFinished( -1 );

    QProcess configTouch;
    configTouch.start( "touch", QStringList() << configFile );
    configTouch.waitForFinished( -1 );

    file = new QFile( configFile );

    while( !file->open( QIODevice::WriteOnly | QIODevice::Append ) );

    file->write( listOfConfigToken[0].toUtf8() );
    file->write( QString::number( listOfConfigToken[1].toLong() - answer.toLong() ).toUtf8() );
    delete file;

    users::UserManager::getInstance()->addUser( newUser );

}





