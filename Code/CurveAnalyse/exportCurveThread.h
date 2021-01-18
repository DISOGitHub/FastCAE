#ifndef EXPORTCURVETHREAD_H
#define EXPORTCURVETHREAD_H

#include <QThread>
#include <QTextCodec>
#include "curve_struct.h"
class ExportCurveThread : public QThread
{
public:
    Q_OBJECT
public:
	ExportCurveThread();
	~ExportCurveThread();
	void initThread(curve_ini_data *curve_data,QString strDir);
	
public:

protected:
    void run();
public slots:
	void stop();
private:
	volatile bool stopped; 
	curve_ini_data *curve_data_ptr;
	QString dirName;
signals:
	void sig_changeExportText(QString);
};

#endif // THRAED_H
