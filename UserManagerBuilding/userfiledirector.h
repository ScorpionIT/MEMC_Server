#ifndef USERFILEDIRECTOR_H
#define USERFILEDIRECTOR_H

#include <QFile>
#include <QString>
#include "userbuilder.h"

namespace UserManagerBuilding
{
    class UserFileDirector
    {
    private:
        UserBuilder* builder;
        QString userFilePath;

    public:
        UserFileDirector( UserBuilder* builder, QString userFilePath );

        void startBuilding();

        ~UserFileDirector();
    };

}
#endif // USERFILEDIRECTOR_H
