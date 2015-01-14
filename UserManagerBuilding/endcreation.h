#ifndef ENDCREATION_H
#define ENDCREATION_H

#include "userbuildingprotocol.h"

namespace UserManagerBuilding
{
    class EndCreation : public UserBuildingProtocol
    {
    private:
        UserBuildingProtocol* next;

    public:
        EndCreation( UserBuildingProtocol* next, UserBuilder* builder );
        virtual bool handle( QString line );
        virtual ~EndCreation();
    };
}
#endif // ENDCREATION_H
