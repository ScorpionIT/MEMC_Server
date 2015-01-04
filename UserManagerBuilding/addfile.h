#ifndef ADDFILE_H
#define ADDFILE_H

#include "userbuildingprotocol.h"

class AddFile : public UserBuildingProtocol
{
private:
    UserBuildingProtocol* next;

public:
    AddFile( UserBuildingProtocol* next, UserBuilder* builder );
    virtual bool handle( QString line );
    ~AddFile();
};

#endif // ADDFILE_H
