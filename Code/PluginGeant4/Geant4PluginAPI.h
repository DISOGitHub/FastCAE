#ifndef _GEANT4PLUGINAPI_H__
#define _GEANT4PLUGINAPI_H__

#include <QtCore/QtGlobal>


#if defined(GEANT4PLUGIN_API)
#define GEANT4PLUGINAPI Q_DECL_EXPORT
#else
#define GEANT4PLUGINAPI Q_DECL_IMPORT
#endif

#endif
