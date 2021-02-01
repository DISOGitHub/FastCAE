QT       += core gui network
TEMPLATE	=   lib
CONFIG		+=  debug  c++11
CONFIG		+=  qt
TARGET		=   PluginTaskCommit
DEFINES     +=  TASK_COMMIT_API
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets core
QMAKE_CXXFLAGS += /MP
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += USE_SOLVER_PLUGIN
#DEFINES += QT_NO_DEBUG_OUTPUT ///
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(taskmgm/taskmgm.pri)

INCLUDEPATH += $$PWD/../../
INCLUDEPATH +=  ./ \
                    ../
Debug::LIBS +=  -L../../output/bin_d
Release::LIBS +=  -L../../output/bin

Debug::LIBS += -lSelfDefObject -lquazipd -lModelData -lModuleBase -lDataProperty -lIO -lMainWidgets -lPluginManager -lBCBase -lMainWindow -lsqlite3_x64
Release::LIBS += -lSelfDefObject -lquazip -lModelData -lModuleBase -lDataProperty -lIO -lMainWidgets -lPluginManager -lBCBase -lMainWindow -lsqlite3_x64

  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d/plugins
  Debug:MOC_DIR         = ./debug/moc
  Debug:RCC_DIR         = ./debug/rcc
  Debug:UI_DIR          = ./debug/qui
  Debug:OBJECTS_DIR     = ./debug/obj

  Release:CONFIG	    	+=  console
  Release:DESTDIR         = ../../output/bin/plugins
  Release:MOC_DIR         = ./debug/moc
  Release:RCC_DIR         = ./debug/rcc
  Release:UI_DIR          = ./debug/qui
  Release:OBJECTS_DIR     = ./debug/obj

SOURCES += \
    CommitTaskDialog.cpp \
    CreatTaskDialog.cpp \
    Dispatcher.cpp \
    DownTaskDialog.cpp \
    LoginDialog.cpp \
    #TaskActionImpl.cpp \
    TaskCommitPlugin.cpp \
    TaskData.cpp \
    TaskMgeWidget.cpp \
    TaskThread.cpp \
#    main.cpp \
    TaskProWidget.cpp \
    Monitortaskinfo.cpp

HEADERS += \
    ActionType.h \
    CommitTaskDialog.h \
    CreatTaskDialog.h \
    Dispatcher.h \
    DownTaskDialog.h \
    LoginDialog.h \
    NonCopyable.h \
    #TaskActionImpl.h \
    TaskCommitPlugin.h \
    TaskData.h \
    TaskMacro.h \
    TaskMgeWidget.h \
    TaskProWidget.h \
    TaskThread.h \
    Monitortaskinfo.h

FORMS += \
    CommitTaskDialog.ui \
    CreatTaskDialog.ui \
    DownTaskDialog.ui \
    LoginDialog.ui \
    TaskMgeWidget.ui \
    TaskProWidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


TRANSLATIONS += TaskCommit_zh_CN.ts

RESOURCES += \
    Translation.qrc \
    res.qrc
