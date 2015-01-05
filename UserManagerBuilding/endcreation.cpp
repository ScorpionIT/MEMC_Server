#include "endcreation.h"
using namespace UserManagerBuilding;


EndCreation::EndCreation( UserBuildingProtocol* next, UserBuilder* builder ) : UserBuildingProtocol( builder ), next( next )
{

}

bool EndCreation::handle( QString line )
{
    if( line == "END" )
    {
        builder->putUser();
        return true;
    }
    else if( next != nullptr )
        return next->handle( line );
    else
        return UserBuildingProtocol::handle( line );
}

EndCreation::~EndCreation()
{

}

