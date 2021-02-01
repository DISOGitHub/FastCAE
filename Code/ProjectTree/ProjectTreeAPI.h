#ifndef _PROJECTTREEAPI_H_
#define _PROJECTTREEAPI_H_

#include <QtCore/QtGlobal>


#if defined(PROJECTTREE_API)
#define PROJECTTREEAPI Q_DECL_EXPORT
#else
#define PROJECTTREEAPI Q_DECL_IMPORT
#endif
 
#endif
