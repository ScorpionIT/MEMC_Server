#include "userfiledirector.h"
#include "endcreation.h"
#include "password.h"
#include "totalmemory.h"
#include "username.h"
#include "addfile.h"

#include "../usermanager.h"

#include <QTextStream>
#include <QDebug>
using namespace UserManagerBuilding;


UserFileDirector::UserFileDirector( UserBuilder* builder, QString userFilePath ) : builder( builder ), userFilePath( userFilePath )
{

}

void UserFileDirector::startBuilding()
{
    EndCreation endCreation( nullptr, builder );
    Password password( &endCreation, builder );
    TotalMemory totMemory( &password, builder );
    AddFile addFile( &totMemory, builder );
    UserName chain( &addFile, builder );

    QFile file( userFilePath );

    if( !file.exists() ){}
    //throw

    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ){}
    //throw

    QTextStream in( &file );

    QString line = in.readLine();

    builder->setEntryPoint( line );

    while ( !in.atEnd() )
    {
        line = in.readLine();

        qDebug() << line;
        if( line == "BEGINNING" )
        {
            continue;
        }

        chain.handle( line );

    }

    file.close();
}

UserFileDirector::~UserFileDirector()
{

}

