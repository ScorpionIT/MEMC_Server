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
    usermanager.h

SOURCES += \
    connectionmanager.cpp \
    connection.cpp \
    main.cpp \
    user.cpp \
    dlnaservice.cpp \
    dlnaprocess.cpp \
    usermanager.cpp
