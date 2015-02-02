#ifndef STREAMINGPROCESS_H
#define STREAMINGPROCESS_H

#include <QFile>

#include "services/genericprocess.h"
#include "usermanager.h"

using namespace users;

namespace services
{
    class StreamingProcess : public GenericProcess
    {

    private:
        void run();
        void closeConnection();
        void return404();

    public:
        StreamingProcess(QTcpSocket *client);
        ~StreamingProcess();
    };
}

#endif // STREAMINGPROCESS_H
