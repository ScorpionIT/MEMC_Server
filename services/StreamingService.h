#ifndef STREAMINGSERVICE_H
#define STREAMINGSERVICE_H

#include "usermanager.h"
#include "services/genericservice.h"
#include "services/StreamingProcess.h"
using namespace users;

namespace services
{

    class StreamingService : public GenericService
    {

    private:
        void run();

    public:
        StreamingService();
    };
}

#endif // STREAMINGSERVICE_H
