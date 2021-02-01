#ifndef _SELFDEFINEOBJAPI_H_
#define _SELFDEFINEOBJAPI_H_

#include <QtCore/QtGlobal>


#if defined(SELFDEFINEOBJ_API)
#define SELFDEFINEOBJAPI Q_DECL_EXPORT
#else
#define SELFDEFINEOBJAPI Q_DECL_IMPORT
#endif

#endif
