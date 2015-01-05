#ifndef USERNAME_H
#define USERNAME_H

#include "userbuildingprotocol.h"
#include <QString>

namespace UserManagerBuilding
{
    class UserName : public UserBuildingProtocol
    {
    private:
        UserBuildingProtocol* next;
    public:
        UserName( UserBuildingProtocol* next, UserBuilder* builder );
        virtual bool handle( QString line );
        ~UserName();
    };
}
#endif // USERNAME_H
