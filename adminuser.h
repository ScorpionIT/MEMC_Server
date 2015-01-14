#ifndef ADMINUSER_H
#define ADMINUSER_H

#include <QString>
#include "user.h"
using namespace users;

class AdminUser : public User
{
private:

public:
    AdminUser();
    ~AdminUser();
};

#endif // ADMINUSER_H
