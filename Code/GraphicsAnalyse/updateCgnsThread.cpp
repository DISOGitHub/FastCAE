#include "updateCgnsThread.h"
#include <QTime>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>

updateCgnsThread::updateCgnsThread()
{
	max_cgns_times = 0;
	cur_cgns_time = 0;
}

updateCgnsThread::~updateCgnsThread()
{
}

void updateCgnsThread::initThread(PipelineObject* pipeObj, int tep_max_cgns_times, int tep_cur_cgns_time)
{
	cur_NumericSrcObj = (NumericSrcObject*)pipeObj;
	max_cgns_times = tep_max_cgns_times;
	cur_cgns_time = tep_cur_cgns_time;
	stopped = false;
	colVal_map.clear();
}

void updateCgnsThread::stop()
{
    stopped=true;
}

PipelineObject* updateCgnsThread::findParentPipelineObj(QString tep_filename, QList<PipelineObject*> tep_mPipelineObjs)
{
	for (int i = 0; i < tep_mPipelineObjs.count(); i++)
	{
		if (tep_filename == tep_mPipelineObjs[i]->mPipeLineObjProp.pipelineObj_base_propData.fileName)
		{
			return tep_mPipelineObjs[i];
		}
			
	}
	return NULL;
}

void updateCgnsThread::slot_update_scalarBar(PipelineObject *tep_pipelineObj)
{
	if (tep_pipelineObj->GetObjectType() != dNumeric_DataSource)
		return;
	QString tep_colorColumn;
	vtkSmartPointer<vtkScalarBarWidget> tep_scalarWidget = NULL;
	
	QMap<QString, bool>::iterator it;
	for (it = tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.begin(); it != tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.end(); it++)
	{
		if (tep_pipelineObj->scalarBar_widgetMap.contains(it.key()))
		{
			tep_scalarWidget = tep_pipelineObj->scalarBar_widgetMap[it.key()];

			if (it.value())
			{
				//动画颜色条范围变化
				QString tep_colorColumn = it.key();
				double mRange[2];
				tep_pipelineObj->getcolorColumnRange(tep_colorColumn, mRange);
				tep_scalarWidget->GetScalarBarActor()->GetLookupTable()->SetRange(mRange);
				//动画颜色条范围变化
			}
		}
	}
}

void updateCgnsThread::run()
{
	QString tep_filename;
	QString tep_str;
	first_frame_xh =cur_cgns_time;
	if (cur_NumericSrcObj == NULL)
		return;
	
	for (int i = first_frame_xh; i < max_cgns_times; i++)
	{
		if (stopped)
		{
			return;
		}
		g_aniThread_mutex.lock();
		colVal_map.clear();
		colVal_map = cur_NumericSrcObj->readdata_io->getCgnsValue(i);
		if (colVal_map.count() < 1)
			continue;
		if (colVal_map.contains(cur_NumericSrcObj->mBaseGridBak))
		{
			cur_NumericSrcObj->blockValMap.clear();
			cur_NumericSrcObj->blockValMap = colVal_map[cur_NumericSrcObj->mBaseGridBak];
		}
		else
			continue;
		if (stopped)
		{
			g_aniThread_mutex.unlock();
			return;
		}
		emit sig_update_cgns_plot((PipelineObject*)cur_NumericSrcObj,i);
		cur_cgns_time=i;
	}
	emit sig_finish_cgnsThread(cur_NumericSrcObj);
}
void updateCgnsThread::slot_update_cubeAxes(PipelineObject *tep_pipelineObj)
{
	if (tep_pipelineObj->GetObjectType() != dNumeric_DataSource)
		return;
	QString tep_colorColumn;
	vtkSmartPointer<vtkScalarBarWidget> tep_scalarWidget = NULL;
	
	QMap<QString, bool>::iterator it;
	for (it = tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.begin(); it != tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.end(); it++)
	{
		if (tep_pipelineObj->scalarBar_widgetMap.contains(it.key()))
		{
			tep_scalarWidget = tep_pipelineObj->scalarBar_widgetMap[it.key()];

			if (it.value())
			{
				//动画颜色条范围变化
				QString tep_colorColumn = it.key();
				double mRange[2];
				tep_pipelineObj->getcolorColumnRange(tep_colorColumn, mRange);
				tep_scalarWidget->GetScalarBarActor()->GetLookupTable()->SetRange(mRange);
				//动画颜色条范围变化
			}
		}
	}
}