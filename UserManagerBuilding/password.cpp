#include "password.h"
using namespace UserManagerBuilding;


Password::Password( UserBuildingProtocol* next, UserBuilder* builder ) : UserBuildingProtocol( builder ), next( next )
{
}

bool Password::handle( QString line )
{
    QStringList tokens = line.split( "passwd=" );

    if( tokens[0] != line )
    {
        builder->createPasswd( tokens[1] );
        return true;
    }
    else if( next != nullptr )
        return next->handle( line );
    else
        return UserBuildingProtocol::handle( line );
}

Password::~Password()
{
    if( next != nullptr )
        delete next;

}

