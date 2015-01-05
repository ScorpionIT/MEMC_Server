#include "username.h"
using namespace UserManagerBuilding;


UserName::UserName(UserBuildingProtocol* next, UserBuilder* builder) : UserBuildingProtocol( builder ), next( next )
{
}

bool UserName::handle( QString line )
{
    QStringList tokens = line.split( "user=" );

    if( tokens[0] != line )
    {
        builder->createUser( tokens[1] );
        return true;
    }
    else if( next != nullptr )
        return next->handle( line );
    else
        return UserBuildingProtocol::handle( line );
}

UserName::~UserName()
{

}

