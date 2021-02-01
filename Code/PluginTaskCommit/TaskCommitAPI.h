#ifndef _TASKCOMMITAPI_H__
#define _TASKCOMMITAPI_H__

#include <QtCore/QtGlobal>


#if defined(TASK_COMMIT_API)
#define TASK_COMMIT Q_DECL_EXPORT
#else
#define TASK_COMMIT Q_DECL_IMPORT
#endif

#endif
