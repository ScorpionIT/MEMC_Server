#ifndef TOTALMEMORY_H
#define TOTALMEMORY_H

#include "userbuildingprotocol.h"
namespace UserManagerBuilding
{
    class TotalMemory : public UserBuildingProtocol
    {
    private:
        UserBuildingProtocol* next;
    public:
        TotalMemory( UserBuildingProtocol* next, UserBuilder* builder );
        virtual bool handle( QString line );
        virtual ~TotalMemory();
    };
}
#endif // TOTALMEMORY_H
