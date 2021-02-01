#ifndef _PLUGINMANAGERAPI_H__
#define _PLUGINMANAGERAPI_H__

#include <QtCore/QtGlobal>


#if defined(PLUGINMANAGER_API)
#define PLUGINMANAGERAPI Q_DECL_EXPORT
#else
#define  PLUGINMANAGERAPI Q_DECL_IMPORT
#endif

#endif
