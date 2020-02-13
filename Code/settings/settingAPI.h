#ifndef _SETTINGAPI_H_
#define _SETTINGAPI_H_

#include <QtCore/QtGlobal>


#if defined(SETTING_API)
#define SETTINGAPI Q_DECL_EXPORT
#else
#define  SETTINGAPI Q_DECL_IMPORT
#endif

#endif
