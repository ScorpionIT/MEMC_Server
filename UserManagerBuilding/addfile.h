#ifndef ADDFILE_H
#define ADDFILE_H

#include "userbuildingprotocol.h"

namespace UserManagerBuilding
{
    class AddFile : public UserBuildingProtocol
    {
    private:
        UserBuildingProtocol* next;

    public:
        AddFile( UserBuildingProtocol* next, UserBuilder* builder );
        virtual bool handle( QString line );
        virtual ~AddFile();
    };
}
#endif // ADDFILE_H
