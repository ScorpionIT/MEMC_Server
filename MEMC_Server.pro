QT += network \
      core

CONFIG += c++11

HEADERS += \
    connectionmanager.h \
    connection.h \
    user.h \
    userservice.h \
    dlnaservice.h \
    dlnaprocess.h \
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
    fileservice.h

SOURCES += \
    connectionmanager.cpp \
    connection.cpp \
    main.cpp \
    user.cpp \
    dlnaservice.cpp \
    dlnaprocess.cpp \
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
    fileservice.cpp
