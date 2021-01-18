#ifndef _USERGUIDENCEAPI_H__
#define _USERGUIDENCEAPI_H__

#include <QtCore/QtGlobal>


#if defined(USERGUIDENCE_API)
#define USERGUIDENCEAPI Q_DECL_EXPORT
#else
#define USERGUIDENCEAPI Q_DECL_IMPORT
#endif

#endif
