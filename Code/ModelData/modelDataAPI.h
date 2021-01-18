#ifndef _MODELDATAAPI_H_
#define _MODELDATAAPI_H_

#include <QtCore/QtGlobal>


#if defined(MODELDATA_API)
#define MODELDATAAPI Q_DECL_EXPORT
#else
#define MODELDATAAPI Q_DECL_IMPORT
#endif

#endif
