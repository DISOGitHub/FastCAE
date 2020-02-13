#ifndef _ETSPLUGINAPI_H__
#define _ETSPLUGINAPI_H__

#include <QtCore/QtGlobal>


#if defined(ETSPLUGIN_API)
#define ETSPLUGINAPI Q_DECL_EXPORT
#else
#define ETSPLUGINAPI Q_DECL_IMPORT
#endif

#endif
