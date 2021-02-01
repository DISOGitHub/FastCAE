#ifndef _MAINWINDOWAPI_H_
#define _MAINWINDOWAPI_H_

#include <QtCore/QtGlobal>


#if defined(MAINWINDOW_API)
#define MAINWINDOWAPI Q_DECL_EXPORT
#else
#define  MAINWINDOWAPI Q_DECL_IMPORT
#endif

#endif
