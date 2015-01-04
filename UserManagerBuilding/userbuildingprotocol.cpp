#include "userbuildingprotocol.h"

UserBuildingProtocol::UserBuildingProtocol( UserBuilder* builder )
{
    this->builder = builder;
}

bool UserBuildingProtocol::handle( QString line )
{
    return false;
}

UserBuildingProtocol::~UserBuildingProtocol()
{

}

