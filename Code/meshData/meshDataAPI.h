#ifndef _MESHDATAAPI_H_
#define _MESHDATAAPI_H_

#include <QtCore/QtGlobal>


#if defined(MESHDATA_API)
#define MESHDATAAPI Q_DECL_EXPORT
#else
#define  MESHDATAAPI Q_DECL_IMPORT
#endif

#endif
