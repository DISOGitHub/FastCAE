#ifndef HEUPROJECTDB_H
#define HEUPROJECTDB_H

#include <qlist.h>
#include <qstring.h>
#include "PipelineObject.h"
#include "FilterClip.h"
#include "FilterContour.h"
#include "FilterSlice.h"
#include "FilterVector.h"

class HeuProjectDB
{
public:
	HeuProjectDB();
	~HeuProjectDB();

	void AddPipelineObject(PipelineObject* pipeObj);
	void RmvPipelineObject(PipelineObject* pipeObj);
	PipelineObject* GetFirstObject();
	bool ContainFilterObj(const QString& name);
	void GetPipelineObjects(QList<PipelineObject*> pipelineObjs);
	QList<PipelineObject*> GetPipelineObjects() { return mPipelnObjs; }
	void update_pipelineObjs(QList<PipelineObject*> pipelineObjs);
private:
	QList<PipelineObject*> mPipelnObjs;
};
#endif

