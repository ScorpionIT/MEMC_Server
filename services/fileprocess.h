#ifndef FILEPROCESS_H
#define FILEPROCESS_H

#include "genericprocess.h"

namespace services
{
class FileProcess : public GenericProcess
{
private:
    virtual void run();
    virtual void closeConnection();

public:
    FileProcess( QTcpSocket* client );
    virtual ~FileProcess();
};
}
#endif // FILEPROCESS_H
