#ifndef EXPORTFILETHREAD_H
#define EXPORTFILETHREAD_H

#include <QThread>
#include <QTextCodec>
#include "NumericSrcObject.h"
#include "PipelineObject.h"
#include "ReadDataIO.h"
#include "FilterContour.h"
#include "FilterClip.h"
#include "FilterSlice.h"
#include "FilterVector.h"
#include "SolutionDataIO/solutiondataio.h"

class ExportFileThread : public QThread
{
public:
    Q_OBJECT
public:
	ExportFileThread();
	~ExportFileThread();
	void initThread(PipelineObject* tep_pipelineObj, QString tep_filename);
    
public:

protected:
    void run();
public slots:
	void stop();
private:
	PipelineObject* cur_pipelineObj;
	QString cur_filename;
    volatile bool stopped; 
	SolutionDataIO tp;
signals:
	void sig_changeExportText(QString);
};

#endif // THRAED_H
