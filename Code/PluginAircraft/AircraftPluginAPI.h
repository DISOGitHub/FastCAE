﻿#ifndef _AIRCRAFTPLUGINAPI_H__
#define _AIRCRAFTPLUGINAPI_H__

#include <QtCore/QtGlobal>


#if defined(AIRCRAFTPLUGIN_API)
#define AIRCRAFTPLUGINAPI Q_DECL_EXPORT
#else
#define AIRCRAFTPLUGINAPI Q_DECL_IMPORT
#endif

#endif
