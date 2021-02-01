#ifndef _CONFIGOPTIONSAPI_H_
#define _CONFIGOPTIONSAPI_H_

#include <QtCore/QtGlobal>


#if defined(CONFIGOPTIONS_API)
#define CONFIGOPTIONSAPI Q_DECL_EXPORT
#else
#define CONFIGOPTIONSAPI Q_DECL_IMPORT
#endif

#endif
