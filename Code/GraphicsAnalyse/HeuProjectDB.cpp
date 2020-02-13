#include "HeuProjectDB.h"
#include "PipelineObject.h"
#include "NumericSrcObject.h"
HeuProjectDB::HeuProjectDB()
{
	mPipelnObjs.clear();
}

HeuProjectDB::~HeuProjectDB()
{
	for (int i = 0; i < mPipelnObjs.size(); i++)
	{
		mPipelnObjs.removeOne(mPipelnObjs[i]);
	}
		
	mPipelnObjs.clear();
}

void HeuProjectDB::AddPipelineObject(PipelineObject* pipeObj)
{
	mPipelnObjs.push_back(pipeObj);
}

void HeuProjectDB::RmvPipelineObject(PipelineObject* pipeObj)
{
	mPipelnObjs.removeOne(pipeObj);
	if(pipeObj!=NULL)//if ((pipeObj->GetObjectType() != dSlice_DataSource) && (pipeObj->GetObjectType() != dClip_DataSource) && (pipeObj != NULL))
	{
		if (pipeObj->GetObjectType()==dNumeric_DataSource)
			((NumericSrcObject*)pipeObj)->delDataSet();
		delete pipeObj;
		pipeObj = NULL;
	}
}

PipelineObject* HeuProjectDB::GetFirstObject()
{
	if (mPipelnObjs.size() > 0)
		return mPipelnObjs[0];
	return 0;
}

bool HeuProjectDB::ContainFilterObj(const QString& name)
{
	for (int i = 0; i < mPipelnObjs.size(); i++)
	{
		if (name.compare(mPipelnObjs[i]->GetName(), Qt::CaseInsensitive) == 0)
		{
			return true;
		}
	}
	return false;
}

void HeuProjectDB::GetPipelineObjects(QList<PipelineObject*> pipelineObjs)
{
	for (int i = 0; i < mPipelnObjs.size(); i++)
	{
		//if (mPipelnObjs[i]->IsOkOnRenderingView())
			pipelineObjs.push_back(mPipelnObjs[i]);
	}
}

void HeuProjectDB::update_pipelineObjs(QList<PipelineObject*> pipelineObjs)
{
	/*QList<PipelineObject*>::iterator item = mPipelnObjs.begin();
	while (item != mPipelnObjs.end())
	{
		mPipelnObjs.removeOne(*item);
		PipelineObject *index = (PipelineObject*)*item;
		delete index;
		index = NULL;
		item++;
	}*/
	mPipelnObjs.clear();
	mPipelnObjs.append(pipelineObjs);
	/*int tep_obj_id;
	for (int j = 0; j < pipelineObjs.count(); j++)
	{
		tep_obj_id = pipelineObjs[j]->mPipeLineObjProp.pipelineObj_base_propData.obj_id;
		for (int k = 0; k<mPipelnObjs.count(); k++)
		{
			if (mPipelnObjs[k]->mPipeLineObjProp.pipelineObj_base_propData.obj_id == tep_obj_id)
			{
				if (pipelineObjs[j]->GetObjectType() == dNumeric_DataSource)
				{
					mPipelnObjs[j]->mPipeLineObjProp.pipelineObj_base_propData = pipelineObjs[k]->mPipeLineObjProp.pipelineObj_base_propData;
					mPipelnObjs[j]->mBaseGrid = pipelineObjs[k]->mBaseGrid;
				}
				else if (pipelineObjs[j]->GetObjectType() == dSlice_DataSource)
				{
					mPipelnObjs[j]->mPipeLineObjProp.pipelineObj_base_propData = pipelineObjs[k]->mPipeLineObjProp.pipelineObj_base_propData;
					mPipelnObjs[j]->mBaseGrid = pipelineObjs[k]->mBaseGrid;
					mPipelnObjs[j]->mPipeLineObjProp.sliceFilter_propData = pipelineObjs[k]->mPipeLineObjProp.sliceFilter_propData;
					((FilterSlice *)mPipelnObjs[j])->plane_interactor = ((FilterSlice *)pipelineObjs[k])->plane_interactor;
				}
				else if (pipelineObjs[j]->GetObjectType() == dClip_DataSource)
				{
					mPipelnObjs[j]->mPipeLineObjProp.pipelineObj_base_propData = pipelineObjs[k]->mPipeLineObjProp.pipelineObj_base_propData;
					mPipelnObjs[j]->mBaseGrid = pipelineObjs[k]->mBaseGrid;
					mPipelnObjs[j]->mPipeLineObjProp.clipFilter_propData = pipelineObjs[k]->mPipeLineObjProp.clipFilter_propData;
					((FilterClip *)mPipelnObjs[j])->plane_interactor = ((FilterClip *)pipelineObjs[k])->plane_interactor;
				}
				else if (pipelineObjs[j]->GetObjectType() == dContour_DataSource)
				{
					mPipelnObjs[j]->mPipeLineObjProp.pipelineObj_base_propData = pipelineObjs[k]->mPipeLineObjProp.pipelineObj_base_propData;
					mPipelnObjs[j]->mBaseGrid = pipelineObjs[k]->mBaseGrid;
					mPipelnObjs[j]->mPipeLineObjProp.contourFilter_propData = pipelineObjs[k]->mPipeLineObjProp.contourFilter_propData;
				}
				else if (pipelineObjs[j]->GetObjectType() == dVector_DataSource)
				{
					mPipelnObjs[j]->mPipeLineObjProp.pipelineObj_base_propData = pipelineObjs[k]->mPipeLineObjProp.pipelineObj_base_propData;
					mPipelnObjs[j]->mBaseGrid = pipelineObjs[k]->mBaseGrid;
					mPipelnObjs[j]->mPipeLineObjProp.vectorFilter_propData = pipelineObjs[k]->mPipeLineObjProp.vectorFilter_propData;
				}
				break;
			}
		}
	}*/
}