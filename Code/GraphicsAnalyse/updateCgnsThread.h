#ifndef UPDATECGNSTHREAD_H
#define UPDATECGNSTHREAD_H

#include <QThread>
#include <QTextCodec>
#include "NumericSrcObject.h"
#include "PipelineObject.h"
#include "ReadDataIO.h"
#include "FilterContour.h"
#include "FilterClip.h"
#include "FilterSlice.h"
#include "FilterVector.h"
#include <QMutex>
extern QList<PipelineObject*> g_ani_objList;
extern QMutex g_aniThread_mutex;

class updateCgnsThread : public QThread
{
public:
    Q_OBJECT
public:
	updateCgnsThread();
	~updateCgnsThread();
	void initThread(PipelineObject* mPipelineObjs, int tep_max_cgns_times, int tep_cur_cgns_time);
    void stop();
	NumericSrcObject* cur_NumericSrcObj;
public:

protected:
    void run();

private:
    int threadID;
    volatile bool stopped;    
    QList<PipelineObject*> source_mPipelineObjs;
	QList<PipelineObject*> filter_mPipelineObjs;
	int max_cgns_times;
	int cur_cgns_time;
	
	PipelineObject* findParentPipelineObj(QString tep_filename, QList<PipelineObject*> tep_mPipelineObjs);
	void slot_update_scalarBar(PipelineObject *tep_pipelineObj);
	void slot_update_cubeAxes(PipelineObject *tep_pipelineObj);
	int first_frame_xh;
	QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkFloatArray>>> colVal_map;
signals:
	void sig_finish_cgnsThread(PipelineObject*);
	void sig_update_cgns_plot(PipelineObject*,int);
};

#endif // THRAED_H
