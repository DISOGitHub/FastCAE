#ifndef _PYTHONAPI_H_
#define _PYTHONAPI_H_

#include <QtCore/QtGlobal>


#if defined(PYTHON_API)
#define PYTHONAPI Q_DECL_EXPORT
#else
#define  PYTHONAPI Q_DECL_IMPORT
#endif

#endif
