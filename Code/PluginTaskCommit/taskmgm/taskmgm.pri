
include(include/include.pri)

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/easydb
INCLUDEPATH += $$PWD/include/httpclient
INCLUDEPATH += $$PWD/include/easydb/sqlite

HEADERS += \
    $$PWD/ParaTask.h \
    $$PWD/TaskBase.h \
    $$PWD/TaskCommit.h

SOURCES += \
    $$PWD/ParaTask.cpp \
    $$PWD/TaskBase.cpp \
    $$PWD/TaskCommit.cpp

