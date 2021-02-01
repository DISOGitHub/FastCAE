#ifndef _MODULEBASEAPI_H_
#define _MODULEBASEAPI_H_

#include <QtCore/QtGlobal>


#if defined(MODULEBASE_API)
#define MODULEBASEAPI Q_DECL_EXPORT
#else
#define  MODULEBASEAPI Q_DECL_IMPORT
#endif

#endif
