#include "userbuildingprotocol.h"
using namespace UserManagerBuilding;


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

