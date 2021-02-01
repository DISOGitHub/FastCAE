#ifndef _DATAPROPERTYAPI_H_
#define _DATAPROPERTYAPI_H_

#include <QtCore/QtGlobal>


#if defined(DATAPROPERTY_API)
#define DATAPROPERTYAPI Q_DECL_EXPORT
#else
#define  DATAPROPERTYAPI Q_DECL_IMPORT
#endif

#endif
