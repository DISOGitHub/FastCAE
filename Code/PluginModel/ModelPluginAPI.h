#ifndef _MODELPLUGINAPI_H__
#define _MODELPLUGINAPI_H__

#include <QtCore/QtGlobal>


#if defined(MODELPLUGIN_API)
#define MODELPLUGINAPI Q_DECL_EXPORT
#else
#define  MODELPLUGINAPI Q_DECL_IMPORT
#endif

#endif
