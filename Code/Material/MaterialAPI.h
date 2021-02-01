#ifndef _MATERIALAAPI_H_
#define _MATERIALAAPI_H_

#include <QtCore/QtGlobal>


#if defined(MATERIAL_API)
#define MATERIALAPI Q_DECL_EXPORT
#else
#define MATERIALAPI Q_DECL_IMPORT
#endif

#endif
