#include "PipelineObject.h"
#include "qlist.h"
#include "qmutex.h"
QList<PipelineObject*> g_ani_objList;
QMutex g_aniThread_mutex;
bool flag_render;
QString openDir;
QString openDisDir;