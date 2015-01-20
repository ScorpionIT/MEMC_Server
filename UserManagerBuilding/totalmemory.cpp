#include "totalmemory.h"
#include <QDebug>
using namespace UserManagerBuilding;



TotalMemory::TotalMemory( UserBuildingProtocol* next, UserBuilder* builder ) : UserBuildingProtocol( builder ), next( next )
{
}

bool TotalMemory::handle(QString line)
{

    QStringList tokens = line.split( "totmemory=" );

    if( tokens[0] != line )
    {

        builder->setTotalMemory( tokens[1] );
        qDebug() << "la memoria totale è " << tokens[1];
        return true;
    }
    else if( next != nullptr )
        return next->handle( line );
    else
        return UserBuildingProtocol::handle( line );
}

TotalMemory::~TotalMemory()
{

}

