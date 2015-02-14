#ifndef USERFILEMANAGER_H
#define USERFILEMANAGER_H

#include "userfilemanagerprocess.h"
#include "genericservice.h"

namespace services
{
    class UserFileManager : public GenericService
    {
        Q_OBJECT

    private:
        void run();

    public:
        UserFileManager();
    };
}
#endif // USERFILEMANAGER_H
