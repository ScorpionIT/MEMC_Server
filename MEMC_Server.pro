QT += network \
      core

CONFIG += c++11

HEADERS += \
    user.h \
    userservice.h \
    usermanager.h \
    mediafile.h \
    UserManagerBuilding/userbuildingprotocol.h \
    UserManagerBuilding/password.h \
    UserManagerBuilding/totalmemory.h \
    UserManagerBuilding/userbuilder.h \
    UserManagerBuilding/userfiledirector.h \
    UserManagerBuilding/username.h \
    UserManagerBuilding/endcreation.h \
    serverapplication.h \
    UserManagerBuilding/addfile.h \
    connection/connection.h \
    connection/connectionmanager.h \
    services/dlnaprocess.h \
    services/dlnaservice.h \
    services/fileservice.h \
    services/loadfileconnection.h \
    services/loadfileservice.h

SOURCES += \
    main.cpp \
    user.cpp \
    usermanager.cpp \
    mediafile.cpp \
    UserManagerBuilding/userbuildingprotocol.cpp \
    UserManagerBuilding/password.cpp \
    UserManagerBuilding/totalmemory.cpp \
    UserManagerBuilding/userbuilder.cpp \
    UserManagerBuilding/userfiledirector.cpp \
    UserManagerBuilding/username.cpp \
    UserManagerBuilding/endcreation.cpp \
    serverapplication.cpp \
    UserManagerBuilding/addfile.cpp \
    connection/connection.cpp \
    connection/connectionmanager.cpp \
    services/dlnaprocess.cpp \
    services/dlnaservice.cpp \
    services/fileservice.cpp \
    services/loadfileconnection.cpp \
    services/loadfileservice.cpp
