#ifndef _POST3DAPI_H_
#define _POST3DAPI_H_

#include <QtCore/QtGlobal>


#if defined(POSTWIDGETS_API)
#define POSTAPI Q_DECL_EXPORT
#else
#define POSTAPI Q_DECL_IMPORT
#endif

#endif
