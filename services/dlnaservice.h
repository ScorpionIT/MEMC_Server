#ifndef DLNASERVICE_H
#define DLNASERVICE_H

#include "dlnaprocess.h"
#include "genericservice.h"

namespace services
{
    namespace dlna
    {
        class DLNAService : public GenericService
        {
        private:
            void run();

        public:
            DLNAService();
            virtual ~DLNAService();
        };
    }
}
#endif // DLNASERVICE_H
