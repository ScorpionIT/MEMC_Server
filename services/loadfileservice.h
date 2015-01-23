#ifndef LOADFILESERVICE_H
#define LOADFILESERVICE_H

#include "loadfileprocess.h"
#include "genericservice.h"

namespace services
{
    class LoadFileService : public GenericService
    {
        Q_OBJECT

    private:
        void run();

    public:
        LoadFileService();
        virtual ~LoadFileService();

    };
}

#endif // LOADFILESERVICE_H
