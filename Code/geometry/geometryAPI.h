#ifndef _GEOMETRYAPI_H_
#define _GEOMETRYAPI_H_

#include <QtCore/QtGlobal>


#if defined(GEOMETRY_API)
#define GEOMETRYAPI Q_DECL_EXPORT
#else
#define  GEOMETRYAPI Q_DECL_IMPORT
#endif

#endif
