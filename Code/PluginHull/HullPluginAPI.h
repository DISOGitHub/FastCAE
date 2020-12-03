#ifndef _HULLPLUGINAPI_H_
#define _HULLPLUGINAPI_H_

#include <QtCore/QtGlobal>


#if defined(HULLCUTPLUGIN_API)
#define HULLPLUGINAPI Q_DECL_EXPORT
#else
#define  HULLPLUGINAPI Q_DECL_IMPORT
#endif

#endif
