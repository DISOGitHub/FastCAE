#ifndef _IOAPI_H_
#define _IOAPI_H_

#include <QtCore/QtGlobal>


#if defined(IO_API)
#define IOAPI Q_DECL_EXPORT
#else
#define  IOAPI Q_DECL_IMPORT
#endif

#endif
