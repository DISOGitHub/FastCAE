#include "exportFileThread.h"
#include <QTime>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>

ExportFileThread::ExportFileThread()
{
}

ExportFileThread::~ExportFileThread()
{
	
}

void ExportFileThread::initThread(PipelineObject* tep_pipelineObj,QString tep_filename)
{
	cur_pipelineObj = tep_pipelineObj;
	cur_filename = tep_filename;
	stopped = false;
}

void ExportFileThread::stop()
{
    stopped=true;
}

void ExportFileThread::run()
{
	if (cur_pipelineObj->mBaseGridBak == NULL)
	{
		emit sig_changeExportText(tr("file exported failure!"));
		return;
	}

	if (!tp.write_Points(cur_filename, cur_pipelineObj->mBaseGridBak, pointsID))
	{
		emit sig_changeExportText(tr("file exported failure!"));
		return;
	}
	if (stopped)
	{
		return;
	}
	if (!tp.write_Cells(cur_filename, cur_pipelineObj->mBaseGridBak, cellsID))
	{
		emit sig_changeExportText(tr("file exported failure!"));
		return;
	}
	if (stopped)
	{
		return;
	}
	if (!tp.write_Points_Value(cur_filename, cur_pipelineObj->mBaseGridBak, " "))
	{
		emit sig_changeExportText(tr("file exported failure!"));
		return;
	}
	if (stopped)
	{
		return;
	}
	if (!tp.write_Cells_Value(cur_filename, cur_pipelineObj->mBaseGridBak, " "))
	{
		emit sig_changeExportText(tr("file exported failure!"));
		return;
	}
	if (stopped)
	{
		return;
	}
	if (cur_pipelineObj->dataSetMap.count() > 0)
	{
		QMap<QString, vtkSmartPointer<vtkIdTypeArray>> zoneMap = cur_pipelineObj->dataSetMap[cur_pipelineObj->mBaseGridBak];
		if (zoneMap.count() > 0)
		{
			if (stopped)
			{
				return;
			}
			if (!tp.write_Zones(cur_filename, zoneMap))
			{
				emit sig_changeExportText(tr("file exported failure!"));
				return;
			}
		}
	}
	if ((cur_pipelineObj->mBlockDataSet != NULL) && (cur_pipelineObj->mBlockDataSet->GetNumberOfBlocks()>1))
	{
		if (stopped)
		{
			return;
		}
		if (!tp.write_Zones(cur_filename, cur_pipelineObj->mBlockDataSet))
		{
			emit sig_changeExportText(tr("file exported failure!"));
			return;
		}
	}
	emit sig_changeExportText(tr("file exported succeed!"));
}
