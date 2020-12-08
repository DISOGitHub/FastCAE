#ifndef _DMESHTOOLSAPI_H__
#define _DMESHTOOLSAPI_H__

#include <QtCore/QtGlobal>


#if defined(D2MESHTOOLS_API)
#define D2MESHTOOLSAPI Q_DECL_EXPORT
#else
#define D2MESHTOOLSAPI Q_DECL_IMPORT
#endif

#endif
