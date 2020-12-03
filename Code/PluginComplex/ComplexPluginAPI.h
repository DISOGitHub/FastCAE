#ifndef _COMPLEXPLUGINAPI_H__
#define _COMPLEXPLUGINAPI_H__

#include <QtCore/QtGlobal>


#if defined(COMPLEXPLUGIN_API)
#define COMPLEXPLUGINAPI Q_DECL_EXPORT
#else
#define  COMPLEXPLUGINAPI Q_DECL_IMPORT
#endif

#endif
