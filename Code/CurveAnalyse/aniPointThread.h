#ifndef ANIPOINTTHREAD_H
#define ANIPOINTTHREAD_H

#include <QThread>
#include <QTextCodec>
#include <QMutex>
#include "curve_struct.h"
#include <QPointF>
#include "qvector.h"
extern QMutex g_aniThread_mutex;
extern curve_aniPoint_struct aniPoint_para;
class aniPointThread : public QThread
{
public:
    Q_OBJECT
public:
	aniPointThread();
	~aniPointThread();
	void initThread(curve_ini_data *tep_data, int max_frames, int frame_xh);
    void stop();
	double func_getAniPoint_coorX(curve_line_data tep_line_data, int tep_xh);
protected:
    void run();

private:
    int threadID;
    volatile bool stopped; 
	int cur_max_frames;
	int cur_frame_xh;
	int first_frame_xh;
	curve_ini_data* curveDataPtr;
	QStringList aniPoint_curveNameList;
	bool flag_finish;
signals:
	void sig_finish_aniThread(bool);
	void sig_refreshView();
	void sig_refreshViewAni();
	void sig_addAniPoint(int,int,int);//当前帧、起始帧，总帧数
	void sig_updateAniPoint(int,int,int);//当前帧、起始帧，总帧数
};

#endif // THRAED_H
