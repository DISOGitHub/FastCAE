#ifndef VTKANITHREAD_H
#define VTKANITHREAD_H

#include <QThread>
#include <QTextCodec>
#include "NumericSrcObject.h"
#include "PipelineObject.h"
#include "ReadDataIO.h"
#include "FilterContour.h"
#include "FilterClip.h"
#include "FilterSlice.h"
#include "FilterVector.h"
#include "LoadActionFile.h"
#include <QMutex>
extern QList<PipelineObject*> g_ani_objList;
extern QMutex g_aniThread_mutex;

class vtkAniThread : public QThread
{
public:
    Q_OBJECT
public:
    vtkAniThread();
    ~vtkAniThread();
	void initThread(QList<PipelineObject*> mPipelineObjs, int max_frames, int frame_xh);
    void stop();
public:

protected:
    void run();
	//void runDisplace(int cur_pos);
private:
    int threadID;
    volatile bool stopped;    
    QList<PipelineObject*> source_mPipelineObjs;
	int cur_max_frames;
	int cur_frame_xh;
	int cur_dis_xh;
	PipelineObject* findParentPipelineObj(QString tep_filename, QList<PipelineObject*> tep_mPipelineObjs);
	void slot_update_scalarBar(PipelineObject *tep_pipelineObj);
	int first_frame_xh;
	void initModelData(NumericSrcObject* tep_numericObj);
	int num_maxDisplace;
	//bool func_PipeObjListDisplace();
signals:
	void sig_finish_aniThread(bool);
	void sig_refreshView(QList<PipelineObject*>);
	void sig_refreshViewAni(int);
	void sig_offPlaneWidget(PipelineObject*);
};

#endif // THRAED_H
