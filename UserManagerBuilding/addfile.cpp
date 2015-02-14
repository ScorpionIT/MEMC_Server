#include "addfile.h"
#include <QFile>
#include <QTextStream>
using namespace UserManagerBuilding;


AddFile::AddFile( UserBuildingProtocol* next, UserBuilder* builder ) : UserBuildingProtocol( builder ), next( next )
{
}

bool AddFile::handle(QString line)
{
    QStringList tokens = line.split( "files=" );

    if( tokens[0] != line )
    {
        QFile file( builder->getEntryPoint() + builder->getCurrentUser()->getUserName() + "/" + tokens[1] );
        QTextStream in( &file );

        if( !file.exists() )
            return false;

        if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
            return false;

        tokens = tokens[1].split( "/" );

        while ( !in.atEnd() )
        {
            QString line = in.readLine();
            builder->addFile( tokens[0] + "/" + line );
        }
        file.close();
        return true;
    }
    else if( next != nullptr )
        return next->handle( line );
    else
        return UserBuildingProtocol::handle( line );
}

AddFile::~AddFile()
{
    if( next != nullptr )
        delete next;
}

