#include "addfile.h"

AddFile::AddFile( UserBuildingProtocol* next, UserBuilder* builder ) : UserBuildingProtocol( builder ), next( next )
{
}

bool AddFile::handle(QString line)
{
    QStringList tokens = line.split( "file=" );

    if( tokens[0] != line )
    {
        builder->addFile( tokens[1] );
        return true;
    }
    else if( next != nullptr )
        return next->handle( line );
    else
        return UserBuildingProtocol::handle( line );
}

AddFile::~AddFile()
{

}

