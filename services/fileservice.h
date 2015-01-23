#ifndef FILESERVICE_H
#define FILESERVICE_H

#include "genericservice.h"

namespace services
{
    class FileService : public GenericService
    {

    private:
        void run();

    public:
        FileService();
        virtual ~FileService();
    };

}
#endif // FILESERVICE_H
