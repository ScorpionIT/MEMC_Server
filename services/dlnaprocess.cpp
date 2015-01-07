#include "dlnaprocess.h"
#include <QProcess>
#include <QStringList>

using namespace services;

void DLNAProcess::run()
{
    QProcess mediaTombProcess;

    QStringList parameters;
    parameters << "-c" << QString( UserManager::getInstance()->getUser( this->user )->getUserDirectory() + "/DLNA/config.xml" );

    for( int i = 0; i < files.size(); i++ )
        parameters << "-a " << files[i];

    mediaTombProcess.start( "mediatomb", parameters );
}

DLNAProcess::DLNAProcess(QStringList files, QString user )
{
    this->user = user;
    this->files = files;
}

DLNAProcess::~DLNAProcess()
{

}

