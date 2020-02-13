#ifndef LOADACTIONFILE_H
#define LOADACTIONFILE_H
#include <qstring.h>
#include <qvector.h>
#include "PipelineObject.h"
#include "global_define.h"
class LoadActionFile
{
public:
	LoadActionFile();
	~LoadActionFile();
	
	bool Load(QString,QString Id_str);
	bool LoadCSV(QString path);
	bool func_setPipeObjDisplaceData(PipelineObject *tep_pipelineObj, int tep_cur_frame_xh, QList<PipelineObject*>objList);
	int func_get_max_num_displace(QList<PipelineObject*> tep_mPipelineObjs);
	QVector<FrameDisplace> displaceList;
};
#endif

