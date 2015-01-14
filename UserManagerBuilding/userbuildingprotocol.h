#ifndef USERBUILDINGPROTOCOL_H
#define USERBUILDINGPROTOCOL_H

#include <QStringList>
#include "userbuilder.h"
#include <QDebug>
namespace UserManagerBuilding
{
    class UserBuildingProtocol
    {
    protected:
        UserBuilder* builder;

    public:
        UserBuildingProtocol( UserBuilder* builder );

        virtual bool handle( QString line );

        virtual ~UserBuildingProtocol();
    };
}
#endif // USERBUILDINGPROTOCOL_H
