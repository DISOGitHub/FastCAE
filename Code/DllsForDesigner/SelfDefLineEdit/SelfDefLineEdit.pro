CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(selfdeflineeditplugin)
TEMPLATE    = lib

HEADERS     = selfdeflineeditplugin.h
SOURCES     = selfdeflineeditplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(selfdeflineedit.pri)

FORMS += \
    selfdeflineedit.ui
