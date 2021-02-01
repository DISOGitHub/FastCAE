#ifndef _MAINWIDGETSEAPI_H_
#define _MAINWIDGETSEAPI_H_

#include <QtCore/QtGlobal>


#if defined(MAINWIDGETS_API)
#define MAINWIDGETSAPI Q_DECL_EXPORT
#else
#define  MAINWIDGETSAPI Q_DECL_IMPORT
#endif

#endif
