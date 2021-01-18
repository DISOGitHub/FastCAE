#ifndef _BCBASEAPI_H_
#define _BCBASEAPI_H_

#include <QtCore/QtGlobal>


#if defined(BCBASE_API)
#define BCBASEAPI Q_DECL_EXPORT
#else
#define BCBASEAPI Q_DECL_IMPORT
#endif

#endif
