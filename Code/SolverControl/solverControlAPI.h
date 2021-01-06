#ifndef _SOLVERCONTROLAPI_H_
#define _SOLVERCONTROLAPI_H_

#include <QtCore/QtGlobal>


#if defined(SOLVERCONTROL_API)
#define SOLVERCONTROLAPI Q_DECL_EXPORT
#else
#define  SOLVERCONTROLAPI Q_DECL_IMPORT
#endif

#endif
