#include "userfiledirector.h"
#include "endcreation.h"
#include "password.h"
#include "totalmemory.h"
#include "username.h"

#include "../usermanager.h"

#include <QTextStream>
#include <QDebug>

UserFileDirector::UserFileDirector( UserBuilder* builder, QString userFilePath ) : builder( builder ), userFilePath( userFilePath )
{

}

void UserFileDirector::startBuilding()
{
    EndCreation endCreation( nullptr, builder );
    Password password( &endCreation, builder );
    TotalMemory totMemory( &password, builder );
    UserName chain( &totMemory, builder );

    QFile file( userFilePath );

    if( !file.exists() );
    //throw

    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) );
    //throw

    QTextStream in( &file );

    while ( !in.atEnd() )
    {
        QString line = in.readLine();

        qDebug() << line;
        if( line == "BEGINNING" )
        {
            continue;
        }

        chain.handle( line );

    }
}

UserFileDirector::~UserFileDirector()
{

}

