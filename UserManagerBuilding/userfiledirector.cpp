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
    EndCreation* endCreation = new EndCreation( nullptr, builder );
    Password* password = new Password( endCreation, builder );
    TotalMemory* totMemory = new TotalMemory( password, builder );
    AddFile* addFile = new AddFile( totMemory, builder );
    UserName* chain = new UserName( addFile, builder );

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

        if( line == "BEGINNING" )
        {
            continue;
        }

        chain->handle( line );

    }

    file.close();

//    delete endCreation;
//    delete password;
//    delete totMemory;
//    delete addFile;
    delete chain;
}

UserFileDirector::~UserFileDirector()
{
    delete builder;
}

