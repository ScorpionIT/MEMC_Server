#ifndef PASSWORD_H
#define PASSWORD_H

#include "userbuildingprotocol.h"

namespace UserManagerBuilding
{
    class Password : public UserBuildingProtocol
    {
    private:
        UserBuildingProtocol* next;
    public:
        Password(UserBuildingProtocol* next, UserBuilder* builder );
        virtual bool handle( QString line );
        ~Password();
    };
}
#endif // PASSWORD_H
